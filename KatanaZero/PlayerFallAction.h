#pragma once
#include "Action.h"
class PlayerFallAction : public Action
{
	int way;
	float speed;
public:
	PlayerFallAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

