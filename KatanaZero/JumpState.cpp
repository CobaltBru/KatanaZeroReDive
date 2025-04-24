#include "JumpState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"
#include "EffectManager.h"


PlayerState* JumpState::GetInput(Player* player)
{
    if (player->GetRigidBody()->IsGround() == true)
    {
        return player->GetStates()->Idle;
    }

    const FLineResult lineResult = player->GetRigidBody()->GetResult();
    if (lineResult.LineType == ELineType::Wall)     // if the player is attached to the wall
    {
        // if the previous state was wallslide, automatically switch to wallslide when the player is attached
        if (player->GetInfo()->prevState == "wallslide")
            return player->GetStates()->WallSlide;

        // if the player press toward the wall
        if ((lineResult.IsLeft && KeyManager::GetInstance()->IsStayKeyDown('A')) ||
            (!lineResult.IsLeft && KeyManager::GetInstance()->IsStayKeyDown('D')))
        {
            if (player->GetRigidBody()->IsGround() == false)
            {
                return player->GetStates()->WallSlide;
            }

            if (lineResult.IsLeft) player->SetDirection(EDirection::Left);
            else player->SetDirection(EDirection::Right);
        }

    }   
    
    if (KeyManager::GetInstance()->IsStayKeyDown('A') && player->GetInfo()->prevState != "wallslide")
    {
        player->SetDirection(EDirection::Left);
        player->GetRigidBody()->AddVelocity({ -1.f, 0.f });        
    }
    
    if (KeyManager::GetInstance()->IsStayKeyDown('D') && player->GetInfo()->prevState != "wallslide")
    {
        player->SetDirection(EDirection::Right);
        player->GetRigidBody()->AddVelocity({ 1.f, 0.f });
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON) && player->GetInfo()->bCanAttack)
    {
        player->SetSwitchTime(0.02f);
        return player->GetStates()->Attack;
    }    

    //if (player->GetInfo()->bIsJump == false)
    //{
    //    player->SetSwitchTime(0.02f);
    //    return player->GetStates()->Idle;
    //}
    return nullptr;
}

void JumpState::Enter(Player* player)
{
    //player->GetInfo()->bIsJump = true;
    player->SetFrameIndex(0);
    player->SetSwitchTime(0.02f * 10.f);

    player->SetImage(ImageManager::GetInstance()->FindImage("zerojump"));
    EffectManager::GetInstance()->Activefx("jumpcloud", player->GetPos(), 0.0f, false);
    //EffectManager::GetInstance()->Activefx("jumpcloud", player->GetPos(), player->GetPos() + FPOINT{0.f, -100.f}, 100.0f, false);
    player->GetRigidBody()->AddVelocity({ 0.f, -800.f });
}

void JumpState::Update(Player* player)
{
    
    /*if (player->GetFrameIndex() >= ImageManager::GetInstance()->FindImage("zerojump")->GetMaxFrameX()-1)    
        player->GetInfo()->bIsJump = false;*/
    player->GetRigidBody()->AddVelocity({ 0.f, 0.005f });
}
