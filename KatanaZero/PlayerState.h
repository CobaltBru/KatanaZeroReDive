#pragma once
#include "PlayerInput.h"

enum class EPlayerState
{
	Idle,
	IdleToRun,
	Run,
	RunToIdle,
	Flip,
	Walk,
	Crouch,
	Jump,
	Fall,
	Attack,
	SlowTime,	
	End
};


// anim 사이에 direction 나타내는 정보있으면 좋겠다.
inline void ChangeState(EPlayerState& currPlayerState, std::vector<EPlayerState>& newStates)
{
	if (currPlayerState == EPlayerState::Attack ) return;

	for (EPlayerState newState : newStates)
	{
		if (newState == EPlayerState::Attack)
		{
			currPlayerState = newState;
			return;
		}
		
	}

}

