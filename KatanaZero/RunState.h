#pragma once
#include "PlayerState.h"


class RunState : public PlayerState
{
private:

public:
	PlayerState* GetInput(Player* player);

	void Enter(Player* player);
	void Update(Player* player);
};
