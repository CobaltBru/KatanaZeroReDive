#pragma once
#include "Action.h"

class PlayerLowAction : public Action
{
public:
	PlayerLowAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

