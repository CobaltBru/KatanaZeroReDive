#include "SpriteAnimator.h"
#include "ImageManager.h"
#include "PlayerAnim.h"
#include "Image.h"

HRESULT PlayerAnim::Init()
{
	idleToRunThreshold = 0.1f;

	InitStateTransitionTable();

	// Init(Image* image, int frames, bool IsLoop, float totalTime);
	idleAnim = new SpriteAnimator;
	Image* idleImage = ImageManager::GetInstance()->FindImage("zeroidle");	
	idleAnim->Init(idleImage, idleImage->GetMaxFrameX(), true);
	
	idleToRunAnim = new SpriteAnimator;
	Image* idleToRunImage = ImageManager::GetInstance()->FindImage("zeroidletorun");
	idleToRunAnim->Init(idleToRunImage, idleToRunImage->GetMaxFrameX(), false);
	
	runToIdleAnim = new SpriteAnimator;
	Image* runToIdleImage = ImageManager::GetInstance()->FindImage("zeroruntoidle");
	runToIdleAnim->Init(runToIdleImage, runToIdleImage->GetMaxFrameX(), false);
	
	walkAnim = new SpriteAnimator;
	Image* walkImage = ImageManager::GetInstance()->FindImage("zerowalk");
	walkAnim->Init(walkImage, walkImage->GetMaxFrameX(), true);
	
	flipAnim = new SpriteAnimator;
	Image* flipImage = ImageManager::GetInstance()->FindImage("zeroflip");
	flipAnim->Init(flipImage, flipImage->GetMaxFrameX(), false, 3.0f);

	crouchAnim = new SpriteAnimator;
	Image* downImage = ImageManager::GetInstance()->FindImage("zerocrouch");
	crouchAnim->Init(downImage, downImage->GetMaxFrameX(), false, 1.0f);
	
	jumpAnim = new SpriteAnimator;
	Image* jumpImage = ImageManager::GetInstance()->FindImage("zerojump");
	jumpAnim->Init(jumpImage, jumpImage->GetMaxFrameX(), false, 3.0f);
	
	attackAnim = new SpriteAnimator;
	Image* attackImage = ImageManager::GetInstance()->FindImage("zeroattack");
	attackAnim->Init(attackImage, attackImage->GetMaxFrameX(), false, 3.0f);
	
	return S_OK;
}

void PlayerAnim::Release()
{
	if (idleAnim)
	{
		delete idleAnim;
		idleAnim = nullptr;
	}
	if (idleToRunAnim)
	{
		delete idleToRunAnim;
		idleToRunAnim = nullptr;
	}
	if (runToIdleAnim)
	{
		delete runToIdleAnim;
		runToIdleAnim = nullptr;
	}
	if (walkAnim)
	{
		delete walkAnim;
		walkAnim = nullptr;
	}
	if (crouchAnim)
	{
		delete crouchAnim;
		crouchAnim = nullptr;
	}
	if (jumpAnim)
	{
		delete jumpAnim;
		jumpAnim = nullptr;
	}
	if (attackAnim)
	{
		delete attackAnim;
		attackAnim = nullptr;
	}
}

void PlayerAnim::Update()
{
}

void PlayerAnim::Render(HDC hdc)
{
}

void PlayerAnim::InitStateTransitionTable()
{
	
	stateTransitionTable[(int)EPlayerState::Idle][(int)EPlayerState::IdleToRun] = true;
	stateTransitionTable[(int)EPlayerState::IdleToRun][(int)EPlayerState::Run] = true;
	stateTransitionTable[(int)EPlayerState::Run][(int)EPlayerState::RunToIdle] = true;
	stateTransitionTable[(int)EPlayerState::RunToIdle][(int)EPlayerState::Idle] = true;

	stateTransitionTable[(int)EPlayerState::Idle][(int)EPlayerState::Jump] = true;
	stateTransitionTable[(int)EPlayerState::Jump][(int)EPlayerState::Idle] = true;
	stateTransitionTable[(int)EPlayerState::IdleToRun][(int)EPlayerState::Jump] = true;
	stateTransitionTable[(int)EPlayerState::Jump][(int)EPlayerState::IdleToRun] = true;
	stateTransitionTable[(int)EPlayerState::Run][(int)EPlayerState::Jump] = true;
	stateTransitionTable[(int)EPlayerState::Jump][(int)EPlayerState::Run] = true;
	stateTransitionTable[(int)EPlayerState::RunToIdle][(int)EPlayerState::Jump] = true;
	stateTransitionTable[(int)EPlayerState::Jump][(int)EPlayerState::RunToIdle] = true;
}
