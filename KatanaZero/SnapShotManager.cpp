#include "SnapShotManager.h"
#include "GameObject.h"
#include "TaeKyungObject.h"
#include "TestObject.h"
#include "ScrollManager.h"
#include "ScreenEffectManager.h"
#include "Effect.h"
#include "EffectManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemies.h"
#include "ReplayObjects.h"
#include "SimpleObject.h"
#include <unordered_set>
#include "SceneManager.h"


void SnapShotManager::Init()
{
	saveElapsed = fxElapsed = replayElapsed = fxReplayElapsed = 0.f;
	Save();
	SaveEffects();
}

void SnapShotManager::Release()
{
	for (auto& kv : fxCloneMap) delete kv.second;
	fxCloneMap.clear();
	for (auto* clone : replayClones) delete clone;
	replayClones.clear();
	snapShots.Clear();
	if (player)
		player = nullptr;
	ReleaseInstance();
}

void SnapShotManager::Update(bool isDead)
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	if (!isDead)
	{
		// 저장 주기 분리
		saveElapsed += dt;
		if(saveElapsed >= mainInterval) {
			Save(); 
			saveElapsed = 0.f;
		}

		fxElapsed += dt;
		if(fxElapsed >= fxInterval) {
			SaveEffects(); 
			fxElapsed = 0.f;
		}
	}
	else
	{
		StartReplay();
		if (!isReplaying) return;
		
		replayElapsed += dt;
		if(replayElapsed >= mainInterval / 15.f) {
			
			Replay();
			if (!isReplaying)
				return;
			replayElapsed = 0.f;
		}
		fxReplayElapsed += dt;
		if(fxReplayElapsed >= fxInterval / 15.f) {
			ReplayEffects();
			fxReplayElapsed = 0.f;
		}
	}
}

void SnapShotManager::Save()
{
	PlayerSnapShot pSnapShot;
	vector<EnemySnapShot> eSnapShots;
	
	for (auto* obj : ObjectManager::GetInstance()->GetObjects())
	{
		if (auto* player = dynamic_cast<Player*>(obj))
		{
			player->MakeSnapShot(&pSnapShot);
		}
		else if (auto* enemy = dynamic_cast<Enemy*>(obj)) {
			EnemySnapShot eSnap;
			enemy->MakeSnapShot(&eSnap);
			eSnapShots.push_back(eSnap);
		}
	}
	
	ScrollSnapShot sSnapShot;
	sSnapShot.scroll = ScrollManager::GetInstance()->GetScroll();

	snapShots.SaveSnapShot(pSnapShot, eSnapShots, sSnapShot);
}

void SnapShotManager::SaveEffects()
{
	vector<EffectSnapShot> fxSnapShots;

	for (auto* fx : EffectManager::GetInstance()->GetCurrFx())
	{
		EffectSnapShot fxSnap;
		fx->MakeSnapShot(&fxSnap);
		fxSnapShots.push_back(fxSnap);
	}
	snapShots.SaveFx(fxSnapShots);
}

void SnapShotManager::StartReplay()
{
	if (isReplaying || snapShots.GetBufferSize() == 0) return;
	isReplaying = true;
	replayIndex = static_cast<int>(snapShots.GetBufferSize()) - 1;
	fxReplayIndex = static_cast<int>(snapShots.GetFxBufferSize()) - 1;
	saveElapsed = 0.f;
	fxElapsed = 0.f;
	replayElapsed = 0.f;
	fxReplayElapsed = 0.f;
	// clear old clones
	for (auto& kv : fxCloneMap) delete kv.second;
	fxCloneMap.clear();
	for (auto* clone : replayClones) delete clone;
	replayClones.clear();
	unordered_set<int> allIDs;
	for (int i = 0; i < snapShots.GetFxBufferSize(); ++i)
	{
		for (auto& s : snapShots.GetFxFrame(i))
			allIDs.insert(s.id);
	}
	for (int id : allIDs) {
		auto* clone = new ReplayEffect();
		clone->SetID(id);        // ReplayEffect에 SetID 메서드 추가
		fxCloneMap[id] = clone;
	}
	const auto& frame = snapShots.GetFrame(0);
	{
		auto* rp = new ReplayPlayer();
		rp->ApplySnapShot(frame.player);
		replayClones.push_back(rp);
	}
	for (const auto& eSnap : frame.enemies) {
		auto* re = new ReplayEnemy();
		re->ApplySnapShot(eSnap);
		replayClones.push_back(re);
	}

	ScreenEffectManager::GetInstance()->StartDistortion();
}

void SnapShotManager::Replay()
{
	if (replayIndex < 0)
	{
		isReplaying = false;
		ScreenEffectManager::GetInstance()->StopDistortion();
		snapShots.Clear();
		for (auto* clone : replayClones) delete clone;
		replayClones.clear();
		return;
	}
	size_t eIdx = 0;
	const auto& frame = snapShots.GetFrame(replayIndex);
	for (auto* clone : replayClones) {
		if (auto* rp = dynamic_cast<ReplayPlayer*>(clone)) {
			rp->ApplySnapShot(frame.player);
		}
		if (auto* re = dynamic_cast<ReplayEnemy*>(clone)) {
			if (eIdx < frame.enemies.size()) re->ApplySnapShot(frame.enemies[eIdx++]);
		}
		
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, clone);
	}
	ScrollManager::GetInstance()->ReplayScroll(frame.scroll.scroll);
	--replayIndex;
}

void SnapShotManager::ReplayEffects()
{
	if (fxReplayIndex < 0) return;
		

	const auto& fxframe = snapShots.GetFxFrame(fxReplayIndex);
	unordered_map<int, EffectSnapShot> snapMap;
	for (auto& fxSnap : fxframe) {
		snapMap[fxSnap.id] = fxSnap;
	}
	for (auto& kv : fxCloneMap) {
		int id = kv.first;
		auto* clone = kv.second;
		auto it = snapMap.find(id);
		if (it != snapMap.end()) {
			clone->ApplySnapShot(it->second);
			RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, clone);
		}
	}
	--fxReplayIndex;
}

GameObject* SnapShotManager::GetPlayer()
{
	return ObjectManager::GetInstance()->GetPlayer();
}
