#include "PlayerAttackAction.h"
#include "Player.h"
#include "RigidBody.h"
#include "CommonFunction.h"
#include "SpriteAnimation.h"
#include "EffectManager.h"
#include "Animator.h"

PlayerAttackAction::PlayerAttackAction(Player* player)
{
	this->player = player;
	delay = 0.4f;
	timer = 0;
}
void PlayerAttackAction::onEnter()
{
	FPOINT attackVec = { 0.f,0.f };
	attackVec.x = g_ptMouse.x - player->GetPos().x - ScrollManager::GetInstance()->GetScroll().x;
	attackVec.y = g_ptMouse.y - player->GetPos().y - ScrollManager::GetInstance()->GetScroll().y;
	Normalize(attackVec);
	if (attackVec.x > 0)
	{
		player->SetFlip(false);
		*player->GetWay() = 1;
	}
	else
	{
		player->SetFlip(true);
		*player->GetWay() = -1;
	}
	player->GetAnimator()->startAnimation("attack");
	player->GetRigidBody()->SetVelocity({0.f,0.f});
	player->GetRigidBody()->AddVelocity(attackVec * 600.f);

	// effect
	float fxAngle = atan2f(attackVec.y, attackVec.x) * (180.f / 3.14159265f);
	float speed = sqrt(player->GetRigidBody()->GetVelocity().x * player->GetRigidBody()->GetVelocity().x +
		player->GetRigidBody()->GetVelocity().y * player->GetRigidBody()->GetVelocity().y);
	bool fllip = player->GetFlip();
	EffectManager::GetInstance()->Activefx("normalslash", player->GetPos(), fxAngle, SnapShotManager::GetInstance()->GetPlayer(), player->GetFlip());

}

void PlayerAttackAction::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	FPOINT currentV = player->GetRigidBody()->GetVelocity();
	if (timer >= delay)
	{
		timer = 0;
		if (player->GetRigidBody()->IsGround())
		{
			player->GetAnimator()->startAnimation("idle");
			player->GetRigidBody()->SetVelocity({ currentV.x * 0.1f,0.f });
			player->changeState(STATE::IDLE);
		}
		else
		{
			player->GetAnimator()->startAnimation("fall");
			player->changeState(STATE::AIR);
		}
		
	}
}

void PlayerAttackAction::onExit()
{
}
