#pragma once
#include "config.h"

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
	//float alpha;
	int ID;
	bool isActive;
};

struct SnapShot
{
	PlayerSnapShot player;
	vector<EnemySnapShot> enemies;
	vector<EffectSnapShot> effects;
};