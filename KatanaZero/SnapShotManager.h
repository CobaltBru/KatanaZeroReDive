#pragma once
#include "Singleton.h"
#include "TimeLineBuffer.h"
#include "config.h"
#include <list>

enum class EObjectClassType : uint8_t
{
	Player,
	Enemy,
	Effect,
	End
};

class GameObject;
class SnapShotManager : public Singleton<SnapShotManager>
{
public:
	void Init();
	void Release();
	void Update(bool isDead);
	void AddGameObject(EObjectClassType  InClassType, GameObject* InGameObject) { GameObjectList[(int)InClassType].push_back(InGameObject); }
	void Save();
	void Replay();
	inline bool IsReplaying() { return isReplaying; }
private:
	float elapsedTime{0.f};
	bool isReplaying{ false };
	int replayIndex{ -1 };
	TimeLineBuffer snapShots;
	list<GameObject*> GameObjectList[(int)EObjectClassType::End];
};

