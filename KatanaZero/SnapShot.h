#pragma once
#include "config.h"

struct PlayerSnapShot
{
	FPOINT pos;
	int animFrame;
	bool bFlip;
	string animKey;
};

struct EnemySnapShot
{
	FPOINT pos;
	int animFrame;
	bool bFlip;
	bool isDead;
	string animKey;
};

struct EffectSnapShot
{
	FPOINT startpos;
	FPOINT destpos;
	int animFrame;
	float angle;
	bool bFlip;
	bool isActive;
	string effectKey;
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

