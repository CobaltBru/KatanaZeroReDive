#include "PlayerAirAction.h"
#include "Player.h"
#include "RigidBody.h"


PlayerAirAction::PlayerAirAction(Player* player)
{
	this->player = player;
}

void PlayerAirAction::onEnter()
{
	speed = 500.f;
}

void PlayerAirAction::Update()
{
	FPOINT currentV = player->GetRigidBody()->GetVelocity();
	//ÁÂ¿ìÀÌµ¿,¸ØÃã
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		way = -1;
		player->GetRigidBody()->AddVelocity({ way * speed,0 });
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		way = 1;
		player->GetRigidBody()->AddVelocity({ way * speed,0 });
	}

	//Á¡ÇÁ³¡
	if (KeyManager::GetInstance()->IsOnceKeyUp('W'))
	{
		player->GetRigidBody()->AddVelocity({ 0,400.f });
	}
	
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->GetRigidBody()->AddVelocity({ 0,300.f });
	}

	if (player->GetRigidBody()->IsGround())
	{
		player->changeState(STATE::IDLE);
	}
	if (currentV.y >= 0)
	{
		//animation
	}
}

void PlayerAirAction::onExit()
{
}
