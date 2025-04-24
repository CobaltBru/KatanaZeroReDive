#pragma once
#include "PlayerInput.h"

enum class EPlayerState
{
	Idle,
	Run,
	Flip,
	Walk,
	Crouch,
	Jump,
	Fall,
	Attack,
	SlowTime,	
	End
};

class Player;
class PlayerState
{
private:

public:
	virtual PlayerState* GetInput(Player* player) = 0;

	virtual void Enter(Player* player) = 0;
	virtual void Update(Player* player) = 0;
};

//// anim ���̿� direction ��Ÿ���� ���������� ���ڴ�.
//inline void ChangeState(EPlayerState& currPlayerState, std::vector<EPlayerState>& newStates)
//{
//	if (currPlayerState == EPlayerState::Attack ) return;
//
//	for (EPlayerState newState : newStates)
//	{
//		if (newState == EPlayerState::Attack)
//		{
//			currPlayerState = newState;
//			return;
//		}
//		
//	}
//
//}

