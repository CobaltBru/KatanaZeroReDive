#include "PlayerAirAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Animator.h"

PlayerAirAction::PlayerAirAction(Player* player)
{
	this->player = player;
	way = player->GetWay();
	isfall = false;
}

void PlayerAirAction::onEnter()
{
	speed = 11000.f;
	isfall = false;
}

void PlayerAirAction::Update()
{
	FPOINT currentV = player->GetRigidBody()->GetVelocity();
	bool left = true;
	bool right = true;
	//ÁÂ¿ìÀÌµ¿,¸ØÃã
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		left = true;
		*way = -1;
		player->SetFlip(true);
		player->GetRigidBody()->AddForce({ *way * speed,0 });
		if (player->GetRigidBody()->IsGround())
		{
			player->GetAnimator()->startAnimation("run");
			player->changeState(STATE::WALK);
		}
	}
	else left = false;
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		right = true;
		*way = 1;
		player->SetFlip(false);
		player->GetRigidBody()->AddForce({ *way * speed,0 });
		if (player->GetRigidBody()->IsGround())
		{
			player->GetAnimator()->startAnimation("run");
			player->changeState(STATE::WALK);
		}
	}
	else right = false;

	//Á¡ÇÁ³¡
	if (KeyManager::GetInstance()->IsOnceKeyUp('W'))
	{
		player->GetRigidBody()->AddVelocity({ 0,400.f });
	}
	
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->GetRigidBody()->AddVelocity({ 0,300.f });
	}
	if (!left && !right)
	{
		if (player->GetRigidBody()->IsGround())
		{
			player->GetAnimator()->startAnimation("idle");
			player->GetRigidBody()->SetVelocity({ currentV.x * 0.2f,0.f });
			player->changeState(STATE::IDLE);
		}
	}
	
	if (player->GetRigidBody()->GetResult().LineType == ELineType::Wall)
	{
		player->GetAnimator()->startAnimation("wall");
		player->GetRigidBody()->SetVelocity({ 0.f,currentV.y * 0.8f });
		player->changeState(STATE::WALL);
	}
	if (!isfall)
	{
		if (currentV.y >= 0)
		{
			isfall = true;
			player->GetAnimator()->startAnimation("fall");
		}
	}
}

void PlayerAirAction::onExit()
{
}
