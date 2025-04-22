#include "FlipState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"


PlayerState* FlipState::GetInput(Player* player)
{
    if (player->GetInfo()->bIsFlip == false)
        return player->GetStates()->Idle;

    return nullptr;
}

void FlipState::Enter(Player* player)
{
    updateCount = 0;

    player->SetFrameIndex(0);
    player->GetInfo()->bIsFlip = true;

    player->SetImage(ImageManager::GetInstance()->FindImage("zeroflip"));
    if (player->GetDirection() == EDirection::Left)    player->GetRigidBody()->AddVelocity({ -100.f, 0.f });
    if (player->GetDirection() == EDirection::Right)    player->GetRigidBody()->AddVelocity({ 100.f, 0.f });
}

void FlipState::Update(Player* player)
{
    if (player->GetFrameIndex() >= player->GetImage()->GetMaxFrameX()-1)
        player->GetInfo()->bIsFlip = false;

    if (player->GetDirection() == EDirection::Left)    player->GetRigidBody()->AddVelocity({ -400.f, 0.f });
    if (player->GetDirection() == EDirection::Right)    player->GetRigidBody()->AddVelocity({ 400.f, 0.f });
}
