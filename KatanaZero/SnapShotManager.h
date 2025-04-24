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
class ReplayBase;
class SnapShotManager : public Singleton<SnapShotManager>
{
public:
	void Init();
	void Release();
	void Update(bool isDead);
	void Save();
	void StartReplay();
	void Replay();
	
	inline bool IsReplaying() { return isReplaying; }
	inline int GetReplayIndex() { return replayIndex; }
	GameObject* GetPlayer();
private:
	float elapsedTime{0.f};
	bool isReplaying{ false };
	int replayIndex{ -1 };
	TimeLineBuffer snapShots;
	GameObject* player{ nullptr };
	vector<ReplayBase*> replayClones;
};

