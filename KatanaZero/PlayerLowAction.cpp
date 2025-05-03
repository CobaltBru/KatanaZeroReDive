#include "PlayerLowAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Animator.h"

PlayerLowAction::PlayerLowAction(Player* player)
{
	this->player = player;
}

void PlayerLowAction::onEnter()
{
	player->GetRigidBody()->SetDown(true);
}

void PlayerLowAction::Update()
{

	

	if (KeyManager::GetInstance()->IsOnceKeyUp('S'))
	{
		
		player->GetAnimator()->startAnimation("idle");
		player->changeState(STATE::IDLE);
		return;
		
		
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
	{
		player->SetFlip(true);
		*player->GetWay() = -1;
		player->changeState(STATE::ROLL);
		return;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('D'))
	{
		player->SetFlip(false);
		*player->GetWay() = 1;
		player->changeState(STATE::ROLL);
		return;
	}
}

void PlayerLowAction::onExit()
{
	player->GetRigidBody()->SetDown(false);
}
