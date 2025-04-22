#pragma once
#include "PlayerState.h"


class IdleState: public PlayerState
{
private:

public:
	PlayerState* GetInput(Player* player);
	
	void Enter(Player* player);
	void Update(Player* player);
};

