#include "PlayerIdleAction.h"
#include "Player.h"
#include "RigidBody.h"

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
		player->changeState(STATE::WALK);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		player->changeState(STATE::WALK);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetRigidBody()->AddVelocity({ 0,jumpForce });
		player->changeState(STATE::AIR);
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		player->changeState(STATE::LOW);
	}


}

void PlayerIdleAction::onExit()
{
}
