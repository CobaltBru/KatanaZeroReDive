#pragma once
#include "Action.h"

class PlayerWallAction : public Action
{
public:
	PlayerWallAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

