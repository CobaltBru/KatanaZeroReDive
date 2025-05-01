#include "PlayerAttackAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "CommonFunction.h"

PlayerAttackAction::PlayerAttackAction(Player* player)
{
	this->player = player;
	delay = 0.2f;
	timer = 0;
}
void PlayerAttackAction::onEnter()
{
	FPOINT attackVec = { 0.f,0.f };
	attackVec.x = g_ptMouse.x - player->GetPos().x;
	attackVec.y = g_ptMouse.y - player->GetPos().y;
	Normalize(attackVec);
	if (attackVec.x > 0)
	{
		*player->GetWay() = 1;
	}
	else
	{
		*player->GetWay() = -1;
	}
	player->GetRigidBody()->SetVelocity({0.f,0.f});
	player->GetRigidBody()->AddVelocity(attackVec * 600.f);
}

void PlayerAttackAction::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer >= delay)
	{
		timer = 0;
		if (player->GetRigidBody()->IsGround())
		{
			player->changeState(STATE::IDLE);
		}
		else
		{
			player->changeState(STATE::AIR);
		}
		
	}
}

void PlayerAttackAction::onExit()
{
}
