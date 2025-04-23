#include "IdleState.h"
#include "Player.h" 
#include "RigidBody.h"


PlayerState* IdleState::GetInput(Player* player)
{
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
        return player->GetStates()->Attack;
    if (KeyManager::GetInstance()->IsOnceKeyDown('A') || KeyManager::GetInstance()->IsStayKeyDown('A'))
    {
        player->SetDirection(EDirection::Left);
        return player->GetStates()->Run;
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown('D') || KeyManager::GetInstance()->IsStayKeyDown('D'))
    {
        player->SetDirection(EDirection::Right);
        return player->GetStates()->Run;
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown('W') && player->GetRigidBody()->IsGround())
        return player->GetStates()->Jump;
    if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
    {
        player->GetRigidBody()->SetDown(true);
        return player->GetStates()->Fall;
    }

    return nullptr;
}

void IdleState::Enter(Player* player)
{
	player->SetEState(EPlayerState::Idle);	

}

void IdleState::Update(Player* player)
{
    if (!player->GetInfo()->bIsShiftChanged) return;

    if (player->GetInfo()->bIsShift)
        player->SetImage(ImageManager::GetInstance()->FindImage("zeroidleshadow"));
    else
	    player->SetImage(ImageManager::GetInstance()->FindImage("zeroidle"));
}
