#include "IdleState.h"
#include "Player.h" 
#include "RigidBody.h"


PlayerState* IdleState::GetInput(Player* player)
{
    if (player->GetInfo()->bIsDead)
        return player->GetStates()->Dead;

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON) && player->GetInfo()->bCanAttack)
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

    const FLineResult lineResult = player->GetRigidBody()->GetResult();
    if (lineResult.LineType == ELineType::Wall)    
        player->GetInfo()->bIsWall = true;
    
    if (KeyManager::GetInstance()->IsOnceKeyDown('W') && player->GetRigidBody()->IsGround())
    {
        player->GetInfo()->prevState = "";
        return player->GetStates()->Jump;
    }

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
    player->SetImage(ImageManager::GetInstance()->FindImage("zeroidle"));
}

void IdleState::Update(Player* player)
{
    player->SetImage(ImageManager::GetInstance()->FindImage("zeroidle"));
    
    if (!player->GetInfo()->bIsShiftChanged) return;

    if (player->GetInfo()->bIsShift)
        player->SetImage(ImageManager::GetInstance()->FindImage("zeroidleshadow"));
}
