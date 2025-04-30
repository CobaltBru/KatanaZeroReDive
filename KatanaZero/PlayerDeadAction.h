#pragma once
#include "Action.h"

class PlayerDeadAction : public Action
{
public:
	PlayerDeadAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

