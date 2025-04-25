#include "WallSlideState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"
#include "EffectManager.h"

PlayerState* WallSlideState::GetInput(Player* player)
{
	if (player->GetInfo()->bIsDead)
		return player->GetStates()->Dead;

	const FLineResult lineResult = player->GetRigidBody()->GetResult();

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetRigidBody()->AddVelocity({ (lineResult.IsLeft == true ? 500.f : -500.f), 0.f });
		player->GetInfo()->prevState = "wallslide";
		return player->GetStates()->Jump;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('A') && !lineResult.IsLeft)
	{
		player->GetRigidBody()->AddVelocity({-80.f,50.f });
		return player->GetStates()->Fall;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('D') && lineResult.IsLeft)
	{
		player->GetRigidBody()->AddVelocity({ 80.f,50.f });
		return player->GetStates()->Fall;
	}
	

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON) && player->GetInfo()->bCanAttack)
	{		
		player->GetRigidBody()->AddVelocity({ (lineResult.IsLeft == true ? 100.f : -100.f), -200.f });
		return player->GetStates()->Attack;
	}

	if (player->GetRigidBody()->IsGround())
		return player->GetStates()->Idle;
	return nullptr;
}

void WallSlideState::Enter(Player* player)
{
	updateCount = 0;
	
	player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslide"));
	player->SetAnimKey("zerowallslide");

	const FLineResult lineResult = player->GetRigidBody()->GetResult();
	if (lineResult.IsLeft) player->SetDirection(EDirection::Left);
	else player->SetDirection(EDirection::Right);			
}

void WallSlideState::Update(Player* player)
{
	updateCount++;

	player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslide"));
	//player->GetRigidBody()->AddVelocity({ 0.f, 50.f });

	// effect
	if (updateCount > (player->GetImage()->GetMaxFrameX() * 100))
	{
		const FLineResult lineResult = player->GetRigidBody()->GetResult();
		if (lineResult.IsLeft)
		{
			EffectManager::GetInstance()->Activefx("wallcloud", player->GetPos() + FPOINT{ -player->GetHalfWidth(), player->GetHalfHeight() * 3 }, 90.f, true);
		}
		else
		{
			EffectManager::GetInstance()->Activefx("wallcloud", player->GetPos() + FPOINT{ player->GetHalfWidth(), player->GetHalfHeight() * 3 }, 270.f, false);
		}

		//EffectManager::GetInstance()->Activefx("dustcloud", player->GetPos() + FPOINT{ 0.f, halfHeight },
		//	player->GetPos() + FPOINT{ 0.f, halfHeight }, 0.1f, false);
		updateCount = 0;
	}

	if (player->GetInfo()->bIsShiftChanged && player->GetInfo()->bIsShift)
		player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslideshadow"));	

	//EffectManager::GetInstance()->Activefx("dustcloud", player->GetPos(), 0.0f, false);
	//EffectManager::Activefx(string key, FPOINT pos, FPOINT dest, float speed, bool bFlip)
}




	//if ((KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('D')) || 
	//	(KeyManager::GetInstance()->IsOnceKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('D')) || 
	//	(KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsOnceKeyDown('D')))
	//{
	//	player->GetRigidBody()->AddVelocity({ 100.f, -200.f });
	//	return player->GetStates()->Jump;
	//}
	//if ((KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('A')) ||
	//	(KeyManager::GetInstance()->IsOnceKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('A')) ||
	//	(KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsOnceKeyDown('A')))
	//{
	//	player->GetRigidBody()->AddVelocity({ -100.f, -200.f });
	//	return player->GetStates()->Jump;
	//}

	//if (player->GetDirection() == EDirection::Left && KeyManager::GetInstance()->IsOnceKeyDown('D'))
	//{
	//	return player->GetStates()->Idle;
	//}
	//if (player->GetDirection() == EDirection::Right && KeyManager::GetInstance()->IsOnceKeyDown('A'))
	//{
	//	return player->GetStates()->Idle;
	//}