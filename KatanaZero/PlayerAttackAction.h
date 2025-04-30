#pragma once
#include "Action.h"

class PlayerAttackAction : public Action
{
public:
	PlayerAttackAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

