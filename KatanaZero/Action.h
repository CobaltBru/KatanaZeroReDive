#pragma once
#include "config.h"

class Player;
class Action
{
public:
	Player* player;
	virtual void onEnter() = 0;
	virtual void Update() = 0;
	virtual void onExit() = 0;
};

