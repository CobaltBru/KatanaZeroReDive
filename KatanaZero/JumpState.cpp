#include "JumpState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"

PlayerState* JumpState::GetInput(Player* player)
{
    if (player->GetInfo()->bIsJump == false)
    {
        player->SetSwitchTime(0.02f);
        return player->GetStates()->Idle;
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
    {
        player->SetSwitchTime(0.02f);
        return player->GetStates()->Attack;
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
    {
        player->SetDirection(EDirection::Left);
        player->GetRigidBody()->AddVelocity({ 0.f, -50.f });        
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown('D'))
    {
        player->SetDirection(EDirection::Right);
        player->GetRigidBody()->AddVelocity({ 0.f, -50.f });        
    }

    return nullptr;
}

void JumpState::Enter(Player* player)
{
    player->SetFrameIndex(0);
    player->SetSwitchTime(2.0f);

    player->SetImage(ImageManager::GetInstance()->FindImage("zerojump"));
    player->GetRigidBody()->AddVelocity({ 0.f, -50.f });
}

void JumpState::Update(Player* player)
{
    
    if (player->GetFrameIndex() >= ImageManager::GetInstance()->FindImage("zerojump")->GetMaxFrameX()-1)    
        player->GetInfo()->bIsJump = false;
    
}
