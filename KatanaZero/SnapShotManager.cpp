#include "SnapShotManager.h"
#include "GameObject.h"
#include "TaeKyungObject.h"
#include "TestObject.h"


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

	snapShots.SaveSnapShot(pSnapShot, eSnapShots, fxSnapShots);
}

void SnapShotManager::Replay()
{
	for (int i = snapShots.GetBufferSize() - 1; i >= 0; i++)
	{
		const SnapShot& frame = snapShots.GetFrame(i);

		for (auto obj : GameObjectList[(int)EObjectClassType::Player])
		{
			TaeKyungObject* p = static_cast<TaeKyungObject*>(obj);
			p->ApplySnapShot(frame.player);
		}
		for (const EnemySnapShot& eSnapShots : frame.enemies)
		{
			for (auto obj : GameObjectList[(int)EObjectClassType::Player])
			{
				TestObject* e = static_cast<TestObject*>(obj);
				
				e->ApplySnapShot(eSnapShots);
			}
		}
		
	}
	Sleep(16);
}
