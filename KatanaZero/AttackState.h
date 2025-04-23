#pragma once
#include "PlayerState.h"


class AttackState: public PlayerState
{
private:
	int updateCount ;
	int imageFrequency;

public:
	PlayerState* GetInput(Player* player) override;

	void Enter(Player* player) override;
	void Update(Player* player) override;

};



