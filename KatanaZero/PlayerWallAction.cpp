#include "PlayerWallAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Animator.h"

PlayerWallAction::PlayerWallAction(Player* player)
{
	this->player = player;
}

void PlayerWallAction::onEnter()
{
	
	this->player->GetRigidBody()->SetAccelerationAlpha({ 0,800.f });
}

void PlayerWallAction::Update()
{
	int* way = player->GetWay();
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		if (*way == 1)
		{
			*way = -1;
			player->GetAnimator()->startAnimation("fall");
			player->GetRigidBody()->AddVelocity({ -50.f,0 });
			player->changeState(STATE::AIR);
		}
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		if (*way == -1)
		{
			*way = 1;
			player->GetAnimator()->startAnimation("fall");
			player->GetRigidBody()->AddVelocity({ 50.f,0 });
			player->changeState(STATE::AIR);
		}
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		if (*way == 1)
		{
			*way = -1;
			player->GetAnimator()->startAnimation("jump");
			player->GetRigidBody()->AddVelocity({ -700.f,-1000.f });
			player->changeState(STATE::AIR);
		}
		else
		{
			*way = 1;
			player->GetAnimator()->startAnimation("jump");
			player->GetRigidBody()->AddVelocity({ 700.f,-1000.f });
			player->changeState(STATE::AIR);
		}
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->GetRigidBody()->AddVelocity({ 0.f,200.f });
	}
	if (player->GetRigidBody()->GetResult().LineType != ELineType::Wall)
	{
		player->changeState(STATE::AIR);
	}
	if (player->GetRigidBody()->IsGround())
	{
		player->GetAnimator()->startAnimation("idle");
		player->changeState(STATE::IDLE);
	}
}

void PlayerWallAction::onExit()
{
	this->player->GetRigidBody()->SetAccelerationAlpha({ 0,1500.f });
}
