#include "PlayerJumpAction.h"
#include "Player.h"
#include "RigidBody.h"

PlayerJumpAction::PlayerJumpAction(Player* player)
{
	this->player = player;
}

void PlayerJumpAction::onEnter()
{
	speed = 500.f;
	jumpForce = -1000.f;
	player->GetRigidBody()->AddVelocity({ 0.f, jumpForce });
	
}

void PlayerJumpAction::Update()
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
		player->changeState(STATE::FALL);
	}
	if (currentV.y >= 0)
	{
		player->changeState(STATE::FALL);
	}
}

void PlayerJumpAction::onExit()
{
}
