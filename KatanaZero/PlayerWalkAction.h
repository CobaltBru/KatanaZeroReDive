#pragma once
#include "Action.h"
class PlayerWalkAction : public Action
{
private:
	int* way;
	float speed;
	float jumpForce;
public:
	PlayerWalkAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

