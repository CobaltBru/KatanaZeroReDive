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


void SnapShotManager::Init()
{
}

void SnapShotManager::Release()
{
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
		if (!isReplaying)
		{
			elapsedTime += dt;
			if (elapsedTime >= 0.01666666667f / 3.f)
			{
				
				elapsedTime = 0.0f;
				Save();
			}
			
		}
	}
	else
	{
		StartReplay();
		if (isReplaying)
		{
			elapsedTime += dt;

			if (elapsedTime >= 0.01666666667f / 27.f)
			{
				
				elapsedTime = 0.0f;
				Replay();
			}
		}
	}
}

void SnapShotManager::Save()
{
	PlayerSnapShot pSnapShot;
	vector<EnemySnapShot> eSnapShots;
	vector<EffectSnapShot> fxSnapShots;
	for (auto* obj : ObjectManager::GetInstance()->GetObjects())
	{
		if (auto* player = dynamic_cast<SimpleObject*>(obj))
		{
			player->MakeSnapShot(&pSnapShot);
		}
		else if (auto* enemy = dynamic_cast<Enemy*>(obj)) {
			EnemySnapShot eSnap;
			enemy->MakeSnapShot(&eSnap);
			eSnapShots.push_back(eSnap);
		}
		/*else if (auto* fx = dynamic_cast<Effect*>(obj)) {
			EffectSnapShot fxSnap;
			fx->MakeSnapShot(&fxSnap);
			fxSnapShots.push_back(fxSnap);
		}*/
	}

	ScrollSnapShot sSnapShot;
	sSnapShot.scroll = ScrollManager::GetInstance()->GetScroll();

	snapShots.SaveSnapShot(pSnapShot, eSnapShots, fxSnapShots, sSnapShot);
}

void SnapShotManager::StartReplay()
{
	if (isReplaying || snapShots.GetBufferSize() == 0) return;
	isReplaying = true;
	replayIndex = static_cast<int>(snapShots.GetBufferSize()) - 1;
	elapsedTime = 0.f;
	// clear old clones
	for (auto* clone : replayClones) delete clone;
	replayClones.clear();
	// instantiate clones based solely on snapshot data
	const auto& frame = snapShots.GetFrame(0);
	// player clone
	{
		auto* rp = new ReplayPlayer();
		rp->ApplySnapShot(frame.player);
		replayClones.push_back(rp);
	}
	// enemy clones
	for (const auto& eSnap : frame.enemies) {
		auto* re = new ReplayEnemy();
		re->ApplySnapShot(eSnap);
		replayClones.push_back(re);
	}
	// effect clones
	/*for (const auto& fxSnap : frame.effects) {
		auto* rf = new ReplayEffect();
		rf->ApplySnapShot(fxSnap);
		replayClones.push_back(rf);
	}*/
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
	const auto& frame = snapShots.GetFrame(replayIndex);
	size_t eIdx = 0, fxIdx = 0;
	for (auto* clone : replayClones) {
		if (auto* rp = dynamic_cast<ReplayPlayer*>(clone)) {
			rp->ApplySnapShot(frame.player);
		}
		if (auto* re = dynamic_cast<ReplayEnemy*>(clone)) {
			if (eIdx < frame.enemies.size()) re->ApplySnapShot(frame.enemies[eIdx++]);
		}
		/*else if (auto* rf = dynamic_cast<ReplayEffect*>(clone)) {
			if (fxIdx < frame.effects.size()) rf->ApplySnapShot(frame.effects[fxIdx++]);
		}*/
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, clone);
	}
	ScrollManager::GetInstance()->ReplayScroll(frame.scroll.scroll);
	--replayIndex;
}

GameObject* SnapShotManager::GetPlayer()
{
	return ObjectManager::GetInstance()->GetPlayer();
}
