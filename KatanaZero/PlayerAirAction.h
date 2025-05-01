#pragma once
#include "Action.h"

class PlayerAirAction : public Action
{
	int* way;
	float speed;
public:
	PlayerAirAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

