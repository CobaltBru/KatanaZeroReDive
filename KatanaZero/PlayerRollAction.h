#pragma once
#include "Action.h"


class PlayerRollAction : public Action
{
public:
	PlayerRollAction(Player* player);
	virtual void onEnter() override;
	virtual void Update() override;
	virtual void onExit() override;
};

