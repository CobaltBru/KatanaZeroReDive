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
	
}

void PlayerLowAction::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyUp('S'))
	{
		player->GetAnimator()->startAnimation("idle");
		player->changeState(STATE::IDLE);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
	{
		player->SetFlip(true);
		*player->GetWay() = -1;
		player->changeState(STATE::ROLL);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('D'))
	{
		player->SetFlip(false);
		*player->GetWay() = 1;
		player->changeState(STATE::ROLL);
	}
}

void PlayerLowAction::onExit()
{
}
