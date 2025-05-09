#pragma once
#include "Action.h"


class PlayerRollAction : public Action
{
	int* way;
	float rollSpeed;
	float delay;
	float timer;

	float jumpForce;

	float remainTimer{ 0.f };
	float remainDuration{ 0.02f };
public:
	PlayerRollAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

