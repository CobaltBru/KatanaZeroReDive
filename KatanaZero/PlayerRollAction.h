#pragma once
#include "Action.h"


class PlayerRollAction : public Action
{
	int* way;
	float rollSpeed;
	float delay;
	float timer;

	float jumpForce;
public:
	PlayerRollAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

