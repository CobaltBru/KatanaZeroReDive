#include "RunState.h"
#include "Player.h"
#include "RigidBody.h"
#include "EffectManager.h"
#include <random>


PlayerState* RunState::GetInput(Player* player)
{
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON) && player->GetInfo()->bCanAttack)
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
    player->SetSwitchTime(0.02f);
    player->SetFrameIndex(0);
    player->SetImage(ImageManager::GetInstance()->FindImage("zeroidletorun"));
    if (player->GetDirection() == EDirection::Left)    player->GetRigidBody()->AddVelocity({ -100.f, 0.f });
    if (player->GetDirection() == EDirection::Right)    player->GetRigidBody()->AddVelocity({ 100.f, 0.f });


    EffectManager::GetInstance()->Activefx("dustcloud", player->GetPos() + FPOINT{ 0.f, player->GetHalfHeight()*2 }, 100.f, false);
}

void RunState::Update(Player* player)
{
    if (player->GetInfo()->bIsShift)
        player->SetImage(ImageManager::GetInstance()->FindImage("zerorunshadow"));
    else
        player->SetImage(ImageManager::GetInstance()->FindImage("zerorun"));

    if (player->GetDirection() == EDirection::Left)    player->GetRigidBody()->AddVelocity({ -300.f, 0.f });
    if (player->GetDirection() == EDirection::Right)    player->GetRigidBody()->AddVelocity({ 300.f, 0.f });

    updateCount = rand();
    if (updateCount % 200 == 0 && player->GetRigidBody()->IsGround())
    {
        float randDist = (float)(rand() % 10);
        EffectManager::GetInstance()->Activefx("dustcloud", player->GetPos() + FPOINT{ 0.f, player->GetHalfHeight() * 2.0f + randDist }, 100.f, false);
        updateCount = 0;
    }
}
