#include "WallSlideState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"


PlayerState* WallSlideState::GetInput(Player* player)
{
	if ((KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('D')) || 
		(KeyManager::GetInstance()->IsOnceKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('D')) || 
		(KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsOnceKeyDown('D')))
	{
		player->GetRigidBody()->AddVelocity({ 100.f, -200.f });
		return player->GetStates()->Jump;
	}
	if ((KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('A')) ||
		(KeyManager::GetInstance()->IsOnceKeyDown('W') && KeyManager::GetInstance()->IsStayKeyDown('A')) ||
		(KeyManager::GetInstance()->IsStayKeyDown('W') && KeyManager::GetInstance()->IsOnceKeyDown('A')))
	{
		player->GetRigidBody()->AddVelocity({ -100.f, -200.f });
		return player->GetStates()->Jump;
	}
	//if (player->GetDirection() == EDirection::Left && KeyManager::GetInstance()->IsOnceKeyDown('D'))
	//{
	//	return player->GetStates()->Idle;
	//}
	//if (player->GetDirection() == EDirection::Right && KeyManager::GetInstance()->IsOnceKeyDown('A'))
	//{
	//	return player->GetStates()->Idle;
	//}

	if (player->GetRigidBody()->IsGround())
		return player->GetStates()->Idle;
	return nullptr;
}

void WallSlideState::Enter(Player* player)
{
}

void WallSlideState::Update(Player* player)
{
	player->GetRigidBody()->AddVelocity({ 0.f, 50.f });

	if (!player->GetInfo()->bIsShiftChanged) return;

	if (player->GetInfo()->bIsShift)
		player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslideshadow"));
	else
		player->SetImage(ImageManager::GetInstance()->FindImage("zerowallslide"));
}
