#include "PlayerFallAction.h"
#include "Player.h"
#include "RigidBody.h"

PlayerFallAction::PlayerFallAction(Player* player)
{
	this->player = player;
}

void PlayerFallAction::onEnter()
{
	way = 0;
	speed = 500.f;
}

void PlayerFallAction::Update()
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
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->GetRigidBody()->AddVelocity({ 0,300.f });
	}

	if (player->GetRigidBody()->IsGround())
	{
		player->changeState(STATE::IDLE);
	}

}

void PlayerFallAction::onExit()
{
}
