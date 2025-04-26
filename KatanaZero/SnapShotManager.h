#pragma once
#include "Singleton.h"
#include "TimeLineBuffer.h"
#include "config.h"
#include <list>
#include <unordered_map>

enum class EObjectClassType : uint8_t
{
	Player,
	Enemy,
	Effect,
	End
};

class GameObject;
class ReplayBase;
class ReplayEffect;
class SnapShotManager : public Singleton<SnapShotManager>
{
public:
	void Init();
	void Release();
	void Update(bool isDead);
	void Save();
	void SaveEffects();
	void StartReplay();
	void Replay();
	void ReplayEffects();
	
	inline bool IsReplaying() { return isReplaying; }
	inline int GetReplayIndex() { return replayIndex; }
	GameObject* GetPlayer();
private:
	float elapsedTime{0.f};
	float saveElapsed{ 0.f };
	float fxElapsed{ 0.f };
	float replayElapsed{ 0.f };
	float fxReplayElapsed{ 0.f };
	float mainInterval{ 0.1666667f * 1.f };
	float fxInterval{ 0.1666667f };
	float fxTimer{ 0.f };
	bool isReplaying{ false };
	int saveCount{ 0 };
	int replayCount{ 0 };
	int replayIndex{ -1 };
	int fxReplayIndex{ -1 };
	TimeLineBuffer snapShots;
	GameObject* player{ nullptr };
	vector<ReplayBase*> replayClones;
	unordered_map<int, ReplayEffect*> fxCloneMap;
};

