#include "PlayerWalkAction.h"
#include "Player.h"
#include "RigidBody.h"
PlayerWalkAction::PlayerWalkAction(Player* player)
{
	this->player = player;
}
void PlayerWalkAction::onEnter()
{
	
	way = 0;
	speed = 500.f;
}

void PlayerWalkAction::Update()
{
	//ÁÂ¿ìÀÌµ¿,¸ØÃã
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		way = -1;
		player->GetRigidBody()->AddVelocity({ way * speed,0 });
	}
	if (KeyManager::GetInstance()->IsOnceKeyUp('A'))
	{
		player->changeState(STATE::IDLE);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		way = 1;
		player->GetRigidBody()->AddVelocity({ way * speed,0 });
	}
	if (KeyManager::GetInstance()->IsOnceKeyUp('D'))
	{
		player->changeState(STATE::IDLE);
	}

	//Á¡ÇÁ
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->changeState(STATE::JUMP);
	}
	//±¸¸£±â
	if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
	{
		player->changeState(STATE::ROLL);
	}
	
}

void PlayerWalkAction::onExit()
{
}
