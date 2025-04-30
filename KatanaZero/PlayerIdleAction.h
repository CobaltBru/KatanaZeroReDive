#pragma once
#include "Action.h"
class PlayerIdleAction : public Action
{

public:
	PlayerIdleAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};
