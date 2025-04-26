#include "DeadState.h"
#include "Player.h"
#include "ImageManager.h"
#include "Image.h"
#include "RigidBody.h"

PlayerState* DeadState::GetInput(Player* player)
{
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON) && player->GetInfo()->bCanAttack)
    {
        // set player pos
        player->GetInfo()->bGameStart = false;
        player->GetInfo()->bIsDead = false;
        player->SetPos(FPOINT{ 300.0f, WINSIZE_Y - 200.0f });
        return player->GetStates()->Idle;
    }
    return nullptr;
}

void DeadState::Enter(Player* player)
{    
    player->SetFrameIndex(0);    
    player->SetImage(ImageManager::GetInstance()->FindImage("zerodead"));   
    animCount = 0;
    FPOINT bounceVec = FPOINT{ player->GetDirection() == EDirection::Left ? 300.f : -300.f, 200.f };
    player->GetRigidBody()->AddVelocity(bounceVec);
}

void DeadState::Update(Player* player)
{
    animCount++;
    if (animCount > ImageManager::GetInstance()->FindImage("zerodead")->GetMaxFrameX() - 1)
        player->SetFrameIndex(ImageManager::GetInstance()->FindImage("zerodead")->GetMaxFrameX() - 1);
}
