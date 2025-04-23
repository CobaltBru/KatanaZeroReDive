#include "WallSlideState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"


PlayerState* WallSlideState::GetInput(Player* player)
{
	const FLineResult lineResult = player->GetRigidBody()->GetResult();

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		player->GetRigidBody()->AddVelocity({ (lineResult.IsLeft ==true ? 300.f: -300.f), -200.f});
		return player->GetStates()->Jump;
	}
	/*if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
	{
		player->GetRigidBody()->AddVelocity({-80.f,50.f });
		return player->GetStates()->Idle;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('D'))
	{
		player->GetRigidBody()->AddVelocity({ 80.f,50.f });
		return player->GetStates()->Idle;
	}*/
	

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
}

void WallSlideState::Update(Player* player)
{
	player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslide"));
	player->GetRigidBody()->AddVelocity({ 0.f, 50.f });

	if (!player->GetInfo()->bIsShiftChanged) return;

	if (player->GetInfo()->bIsShift)
		player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslideshadow"));	
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