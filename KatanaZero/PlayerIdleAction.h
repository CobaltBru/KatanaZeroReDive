#pragma once
#include "Action.h"
class PlayerIdleAction : public Action
{
	float jumpForce;
public:
	PlayerIdleAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};
