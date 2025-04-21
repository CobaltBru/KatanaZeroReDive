#pragma once
#include "PlayerState.h"


class Player;
class FallState: public PlayerState
{	
	private:

	public:
		PlayerState* GetInput(Player * player) override;

		void Enter(Player * player) override;
		void Update(Player * player) override;

};

