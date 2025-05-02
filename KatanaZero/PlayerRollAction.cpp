#include "PlayerRollAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Animator.h"

PlayerRollAction::PlayerRollAction(Player* player)
{
	this->player = player;
	way = player->GetWay();
	rollSpeed = 1500.f;
	delay = 0.25f;
	timer = 0;
	jumpForce = -900.f;
}

void PlayerRollAction::onEnter()
{
	player->GetAnimator()->startAnimation("roll");
	
}


void PlayerRollAction::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	FPOINT currentV = player->GetRigidBody()->GetVelocity();
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetAnimator()->startAnimation("jump");
		player->GetRigidBody()->AddVelocity({ 0,jumpForce });
		player->changeState(STATE::AIR);
	}
	if (timer >= delay)
	{
		timer = 0;
		
		if (!player->GetRigidBody()->IsGround())
		{
			player->GetRigidBody()->SetMaxVelocity({ 800, 1000.f });
			player->GetAnimator()->startAnimation("fall");
			player->changeState(STATE::AIR);
		}
		else
		{
			player->GetRigidBody()->SetMaxVelocity({ 800, 1000.f });
			player->GetAnimator()->startAnimation("idle");
			player->GetRigidBody()->SetVelocity({ currentV.x * 0.1f,0.f });
			player->changeState(STATE::IDLE);

		}
	}
	else
	{
		player->GetRigidBody()->SetMaxVelocity({ 1600.f, 1000.f });
		player->GetRigidBody()->SetVelocity({ *way * rollSpeed,0 });
	}
}

void PlayerRollAction::onExit()
{
}
