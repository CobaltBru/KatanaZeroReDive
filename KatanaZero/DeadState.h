#pragma once
#include "PlayerState.h"

class DeadState : public PlayerState
{
private:
	int animCount;

public:
	PlayerState* GetInput(Player* player);

	void Enter(Player* player);
	void Update(Player* player);
};
