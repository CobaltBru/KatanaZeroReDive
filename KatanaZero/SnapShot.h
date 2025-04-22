#pragma once
#include "config.h"

struct PlayerSnapShot
{
	FPOINT pos;
	//State EState; -> 현재 애니메이션 정보
	int animFrame;
};

struct EnemySnapShot
{
	FPOINT pos;
	//State EState;
	int animFrame;
	int ID;
	bool isDead;
};

struct EffectSnapShot
{
	FPOINT pos;
	//State EState;
	int animFrame;
	//float alpha;
	//int ID;
	bool isActive;
};

struct ScrollSnapShot
{
	FPOINT scroll;
};

struct SnapShot
{
	PlayerSnapShot player;
	vector<EnemySnapShot> enemies;
	vector<EffectSnapShot> effects;
	ScrollSnapShot scroll;
};