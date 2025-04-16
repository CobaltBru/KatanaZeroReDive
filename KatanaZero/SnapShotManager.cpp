#include "SnapShotManager.h"
#include "GameObject.h"
#include "TaeKyungObject.h"
#include "TestObject.h"
#include "ScrollManager.h"
#include "ScreenEffectManager.h"


void SnapShotManager::Init()
{
}

void SnapShotManager::Release()
{
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
			if (elapsedTime >= 0.01666666667f * 5.f)
			{
				Save();
				elapsedTime = 0.0f;
			}
		}
	}
	else
	{
		StartReplay();
		if (isReplaying)
		{
			elapsedTime += dt;

			if (elapsedTime >= 0.01666666667f)
			{
				Replay();
				elapsedTime = 0.0f;
			}
		}
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

void SnapShotManager::StartReplay()
{
	if (isReplaying || snapShots.GetBufferSize() <= 0) return;
	isReplaying = true;
	replayIndex = snapShots.GetBufferSize() - 1;
	elapsedTime = 0.0f;
	ScreenEffectManager::GetInstance()->StartDistortion();
}

void SnapShotManager::Replay()
{
	if (replayIndex < 0)
	{
		isReplaying = false;
		ScreenEffectManager::GetInstance()->StopDistortion();
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
