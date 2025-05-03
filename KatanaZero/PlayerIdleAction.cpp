#include "PlayerIdleAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Animator.h"

PlayerIdleAction::PlayerIdleAction(Player* player)
{
	this->player = player;
}

void PlayerIdleAction::onEnter()
{
	jumpForce = -900.f;
}



void PlayerIdleAction::Update()
{
	

	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		player->GetAnimator()->startAnimation("idle_to_run");
		player->changeState(STATE::WALK);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		player->GetAnimator()->startAnimation("idle_to_run");
		player->changeState(STATE::WALK);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetAnimator()->startAnimation("jump");
		player->GetRigidBody()->AddVelocity({ 0,jumpForce });
		player->changeState(STATE::AIR);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->GetAnimator()->startAnimation("crouch");
		player->changeState(STATE::LOW);
	}


}

void PlayerIdleAction::onExit()
{
}
