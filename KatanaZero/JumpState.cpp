#include "JumpState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"

PlayerState* JumpState::GetInput(Player* player)
{
    if (player->GetRigidBody()->IsGround() == true)
    {
        return player->GetStates()->Idle;
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
    {
        player->SetSwitchTime(0.02f);
        return player->GetStates()->Attack;
    }

    if (KeyManager::GetInstance()->IsStayKeyDown('A'))
    {
        player->SetDirection(EDirection::Left);
        player->GetRigidBody()->AddVelocity({ -1.f, 0.f });        
    }

    if (KeyManager::GetInstance()->IsStayKeyDown('D'))
    {
        player->SetDirection(EDirection::Right);
        player->GetRigidBody()->AddVelocity({ 1.f, 0.f });
    }

    //if (player->GetInfo()->bIsJump == false)
    //{
    //    player->SetSwitchTime(0.02f);
    //    return player->GetStates()->Idle;
    //}

    const FLineResult lineResult = player->GetRigidBody()->GetResult();
    if (lineResult.LineType == ELineType::Wall)
    {
        if (player->GetRigidBody()->IsGround() == false)
            return player->GetStates()->WallSlide;

        if (lineResult.IsLeft)
            player->SetDirection(EDirection::Left);
    }
    return nullptr;
}

void JumpState::Enter(Player* player)
{
    //player->GetInfo()->bIsJump = true;
    player->SetFrameIndex(0);
    player->SetSwitchTime(0.02f * 10.f);

    player->SetImage(ImageManager::GetInstance()->FindImage("zerojump"));
    player->GetRigidBody()->AddVelocity({ 0.f, -300.f });
}

void JumpState::Update(Player* player)
{
    
    /*if (player->GetFrameIndex() >= ImageManager::GetInstance()->FindImage("zerojump")->GetMaxFrameX()-1)    
        player->GetInfo()->bIsJump = false;*/
    player->GetRigidBody()->AddVelocity({ 0.f, 0.005f });
}
