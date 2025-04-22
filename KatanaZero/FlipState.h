#pragma once
#include "PlayerState.h"


class FlipState : public PlayerState
{
private:
	int updateCount;

public:
	PlayerState* GetInput(Player* player);

	void Enter(Player* player);
	void Update(Player* player);
};