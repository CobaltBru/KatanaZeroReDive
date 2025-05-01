#pragma once
#include "Action.h"

class PlayerAttackAction : public Action
{

	float delay;
	float timer;

public:
	PlayerAttackAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

