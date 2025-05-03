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
		return;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		player->GetAnimator()->startAnimation("idle_to_run");
		player->changeState(STATE::WALK);
		return;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetAnimator()->startAnimation("jump");
		player->GetRigidBody()->AddVelocity({ 0,jumpForce });
		player->changeState(STATE::AIR);
		return;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		if (player->GetRigidBody()->GetResult().LineType == ELineType::DownLine)
		{
			player->GetRigidBody()->SetDown(true);
			player->GetAnimator()->startAnimation("fall");
			player->changeState(STATE::AIR);
			return;
		}
		else
		{
			player->GetAnimator()->startAnimation("crouch");
			player->changeState(STATE::LOW);
			return;
		}
		
	}
	if (!player->GetRigidBody()->IsGround())
	{
		player->changeState(STATE::AIR);
		return;
	}

}

void PlayerIdleAction::onExit()
{
}
