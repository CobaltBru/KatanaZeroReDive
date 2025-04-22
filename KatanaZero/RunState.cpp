#include "RunState.h"
#include "Player.h"
#include "RigidBody.h"

PlayerState* RunState::GetInput(Player* player)
{
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
        return player->GetStates()->Attack;
    if (KeyManager::GetInstance()->IsOnceKeyUp('A') || KeyManager::GetInstance()->IsOnceKeyUp('D'))
        return player->GetStates()->Idle;
    if (KeyManager::GetInstance()->IsOnceKeyDown('W') && player->GetRigidBody()->IsGround())
        return player->GetStates()->Jump;
    if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
        return player->GetStates()->Flip;

    return nullptr;
}

void RunState::Enter(Player* player)
{
    player->SetFrameIndex(0);
    player->SetImage(ImageManager::GetInstance()->FindImage("zeroidletorun"));
    if (player->GetDirection() == EDirection::Left)    player->GetRigidBody()->AddVelocity({ -100.f, 0.f });
    if (player->GetDirection() == EDirection::Right)    player->GetRigidBody()->AddVelocity({ 100.f, 0.f });
}

void RunState::Update(Player* player)
{
    player->SetImage(ImageManager::GetInstance()->FindImage("zerorun"));
    if (player->GetDirection() == EDirection::Left)    player->GetRigidBody()->AddVelocity({ -300.f, 0.f });
    if (player->GetDirection() == EDirection::Right)    player->GetRigidBody()->AddVelocity({ 300.f, 0.f });
}
