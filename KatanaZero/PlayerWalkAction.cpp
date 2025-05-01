#include "PlayerWalkAction.h"
#include "Player.h"
#include "RigidBody.h"
PlayerWalkAction::PlayerWalkAction(Player* player)
{
	this->player = player;
	way = player->GetWay();
}
void PlayerWalkAction::onEnter()
{
	
	speed = 14000.f;
	jumpForce = -900.f;
}

void PlayerWalkAction::Update()
{
	FPOINT currentV = player->GetRigidBody()->GetVelocity();
	//ÁÂ¿ìÀÌµ¿,¸ØÃã
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		if (*way == 1)
		{
			player->GetRigidBody()->SetVelocity({ 0.f,0.f });
		}
		*way = -1;
		//player->GetRigidBody()->AddVelocity({ way * speed,0 });
		player->GetRigidBody()->AddForce({ *way * speed,0 });
	}
	if (KeyManager::GetInstance()->IsOnceKeyUp('A'))
	{
		player->GetRigidBody()->SetVelocity({ currentV.x * 0.2f,0.f });
		player->changeState(STATE::IDLE);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		if (*way == -1)
		{
			player->GetRigidBody()->SetVelocity({ 0.f,0.f });
		}
		*way = 1;
		//player->GetRigidBody()->AddVelocity({ way * speed,0 });
		player->GetRigidBody()->AddForce({ *way * speed,0 });
	}
	if (KeyManager::GetInstance()->IsOnceKeyUp('D'))
	{
		player->GetRigidBody()->SetVelocity({ currentV.x * 0.2f,0.f });
		player->changeState(STATE::IDLE);
	}

	//Á¡ÇÁ
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetRigidBody()->AddVelocity({ 0,jumpForce });
		player->changeState(STATE::AIR);
	}
	//±¸¸£±â
	if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
	{
		player->changeState(STATE::ROLL);
	}
	
	if (!player->GetRigidBody()->IsGround())
	{
		player->changeState(STATE::AIR);
	}
}

void PlayerWalkAction::onExit()
{
}
