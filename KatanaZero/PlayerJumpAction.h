#pragma once
#include "Action.h"
class PlayerJumpAction : public Action
{
	int way;
	float jumpForce;
	float speed;
public:
	PlayerJumpAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

