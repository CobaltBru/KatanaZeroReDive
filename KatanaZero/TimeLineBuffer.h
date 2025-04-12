#pragma once
#include "config.h"
#include <deque>

struct PlayerSnapShot
{
	FPOINT pos;
	//State state; -> 현재 애니메이션 정보
	int animFrame;
};

struct EnemySnapShot
{
	FPOINT pos;
	//State state;
	int animFrame;
	int ID;
	bool isDead;
};

struct EffectSnapShot
{
	FPOINT pos;
	//State state;
	int animFrame;
	float alpha;
	int ID;
	bool isActive;
};

struct SnapShot
{
	PlayerSnapShot player;
	vector<EnemySnapShot> enemies;
	vector<EffectSnapShot> effects;
};

class TimeLineBuffer
{
public:
	TimeLineBuffer() {};
	~TimeLineBuffer() {};

	void Init();
	void Release();

	void SaveSnapShot(
		const PlayerSnapShot& player,
		const vector<EnemySnapShot>& enemies,
		const vector<EffectSnapShot>& effects);

	void Replay();
private:
	const size_t MAX_SNAPSHOTS = 300;
	deque<SnapShot> frameBuffer;
};

