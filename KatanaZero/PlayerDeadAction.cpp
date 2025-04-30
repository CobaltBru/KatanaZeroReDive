#include "PlayerDeadAction.h"
#include "Player.h"
#include "RigidBody.h"

PlayerDeadAction::PlayerDeadAction(Player* player)
{
	this->player = player;
}
void PlayerDeadAction::onEnter()
{
	

}



void PlayerDeadAction::Update()
{
}

void PlayerDeadAction::onExit()
{
}
