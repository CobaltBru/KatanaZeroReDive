#include "SnapShotManager.h"
#include "GameObject.h"
#include "TaeKyungObject.h"
#include "TestObject.h"
#include "ScrollManager.h"


void SnapShotManager::Init()
{
}

void SnapShotManager::Release()
{
}

void SnapShotManager::Update(bool isDead)
{
	if (!isDead)
	{
		elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
		if (elapsedTime >= 0.0167f)
		{
			Save();
			elapsedTime = 0.0f;
		}
	}
	else
	{
		if (!isReplaying)
		{
			isReplaying = true;
			replayIndex = snapShots.GetBufferSize() - 1;
		}
		Replay();
	}
}

void SnapShotManager::Save()
{
	PlayerSnapShot pSnapShot;
	vector<EnemySnapShot> eSnapShots;
	vector<EffectSnapShot> fxSnapShots;
	for (auto iter = GameObjectList[(int)EObjectClassType::Player].begin(); iter != GameObjectList[(int)EObjectClassType::Player].end();)
	{
		(*iter)->MakeSnapShot(&pSnapShot);	// 내부 구현 필요, 추상함수
		
		++iter;
	}
	for (auto iter = GameObjectList[(int)EObjectClassType::Enemy].begin(); iter != GameObjectList[(int)EObjectClassType::Enemy].end();)
	{
		EnemySnapShot eSnapShot;
		(*iter)->MakeSnapShot(&eSnapShot);	// 내부 구현 필요, 추상함수
		eSnapShots.push_back(eSnapShot);
		++iter;
	}
	for (auto iter = GameObjectList[(int)EObjectClassType::Effect].begin(); iter != GameObjectList[(int)EObjectClassType::Effect].end();)
	{
		EffectSnapShot fxSnapShot;
		(*iter)->MakeSnapShot(&fxSnapShot);	// 내부 구현 필요, 추상함수
		fxSnapShots.push_back(fxSnapShot);
		++iter;
	}

	ScrollSnapShot sSnapShot;
	sSnapShot.scroll = ScrollManager::GetInstance()->GetScroll();

	snapShots.SaveSnapShot(pSnapShot, eSnapShots, fxSnapShots, sSnapShot);
}

void SnapShotManager::Replay()
{
	if (replayIndex < 0)
	{
		isReplaying = false;
		snapShots.Clear();
		return;
	}
	const SnapShot& frame = snapShots.GetFrame(replayIndex);

	// Player 되감기
	for (auto obj : GameObjectList[(int)EObjectClassType::Player])
	{
		TaeKyungObject* p = static_cast<TaeKyungObject*>(obj);
		p->ApplySnapShot(frame.player);
	}

	// Enemy 되감기
	auto enemyIter = GameObjectList[(int)EObjectClassType::Enemy].begin();
	for (const EnemySnapShot& eSnap : frame.enemies)
	{
		if (enemyIter == GameObjectList[(int)EObjectClassType::Enemy].end()) break;
		TestObject* e = static_cast<TestObject*>(*enemyIter);
		e->ApplySnapShot(eSnap);
		++enemyIter;
	}

	ScrollManager::GetInstance()->ReplayScroll(frame.scroll.scroll);

	--replayIndex;
}
