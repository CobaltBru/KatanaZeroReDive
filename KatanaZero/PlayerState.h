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


// anim ���̿� direction ��Ÿ���� ���������� ���ڴ�.
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

