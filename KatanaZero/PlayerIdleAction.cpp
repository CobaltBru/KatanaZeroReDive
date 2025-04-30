#include "PlayerIdleAction.h"
#include "Player.h"
#include "RigidBody.h"

PlayerIdleAction::PlayerIdleAction(Player* player)
{
	this->player = player;
}

void PlayerIdleAction::onEnter()
{
	FPOINT currentV = player->GetRigidBody()->GetVelocity();
	player->GetRigidBody()->AddVelocity({ -currentV.x * 0.8f,0 });
}



void PlayerIdleAction::Update()
{
	

	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		player->changeState(STATE::WALK);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		player->changeState(STATE::WALK);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('W'))
	{
		player->changeState(STATE::JUMP);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->changeState(STATE::LOW);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON))
	{
		player->changeState(STATE::ATTACK);
	}


}

void PlayerIdleAction::onExit()
{
}
