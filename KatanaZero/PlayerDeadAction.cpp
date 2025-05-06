#include "PlayerDeadAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Animator.h"

PlayerDeadAction::PlayerDeadAction(Player* player)
{
	this->player = player;
	toGround = false;
}
void PlayerDeadAction::onEnter()
{
	if (player->GetRigidBody()->IsGround())
	{
		toGround = true;
		player->GetAnimator()->startAnimation("hurtground");
	}
	else
	{
		player->SetFlip((*player->GetWay()) == 1? true:false);
		toGround = false;
		player->GetAnimator()->startAnimation("hurtfly_begin");
	}

}



void PlayerDeadAction::Update()
{
	if (toGround == false)
	{
		if (player->GetRigidBody()->IsGround())
		{
			toGround = true;
			player->GetAnimator()->startAnimation("hurtground");
		}
	}
}

void PlayerDeadAction::onExit()
{
}
