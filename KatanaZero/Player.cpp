#include "Player.h"
#include "Collider.h"
#include "ImageManager.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "PlayerState.h"
#include "SpriteAnimator.h"

Player::Player()
{
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	image = ImageManager::GetInstance()->FindImage("zeroidle");

	Pos = FPOINT{ 300.0f, 300.0f };
	switchTime = 0.1f;

	playerCollider = new Collider(this, EColliderType::Rect, {}, 30.0f, true, 1.0f);
	CollisionManager::GetInstance()->AddCollider(playerCollider, ECollisionGroup::Player);

	// set player input key
	playerInput = new PlayerInput();
	playerInput->Init();

	// bind input action to state function
	InitBindState();

	velocity = FPOINT{ 0.1f, 0.1f };
	accel = FPOINT{ 0.0f, 0.0f };
	addAccel = FPOINT{ 0.01f, 0.01f };

	return S_OK;
}


void Player::Release()
{
	if (playerCollider)
	{
		delete playerCollider;
		playerCollider = nullptr;
	}
	if (playerInput)
	{
		delete playerInput;
		playerInput = nullptr;
	}
}

void Player::Update()
{
	playerInput->UpdateKeystate();
	frameTimer += TimerManager::GetInstance()->GetDeltaTime();	
	
	// get the pressed keys 
	std::vector<EInputAction> actions = playerInput->GetActions();
	for (EInputAction action : actions)
	{

		// action이 attack인 경우 func이 몇번 더 돌아야함
		stateFunction func = inputStateMap[action];
		func(*this);
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Player::Render(HDC hdc)
{
	if (image != nullptr)
	{		

		int FrameIndexMax = image->GetMaxFrameX();
		image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0);

		if (frameTimer > switchTime)
		{
			FrameIndex++;
			frameTimer = 0.0f;
		}
		
		if (FrameIndex >= FrameIndexMax)
		{
			FrameIndex %= FrameIndexMax;
			image = ImageManager::GetInstance()->FindImage("zeroidle");		// attack인 경우
		}
	}
}

void Player::MakeSnapShot(void* out)
{
}

void Player::Left()
{
	Pos.x -= velocity.x;
}

void Player::Right()
{
	Pos.x += velocity.x;
}

void Player::Down()
{
	Pos.y += velocity.y;
}

void Player::Jump()
{
	Pos.y -= velocity.y;
}

void Player::Fall()
{
}

void Player::Attack()
{
	// frame 끝날때까지 pos 바꿔 줘야함
	// deltaTime 고려 -> 1초에 걸쳐서 frame 한바퀴 돌 수 있도록
	Pos.x += velocity.x * 100.0f;
	Pos.y -= velocity.y * 100.0f;

	image = ImageManager::GetInstance()->FindImage("zeroattack");
}

void Player::Dead()
{
}

void Player::WallSlide()
{
}

void Player::InitBindState()
{
	inputStateMap[EInputAction::Left] = &Player::Left;
	inputStateMap[EInputAction::Right] = &Player::Right;
	inputStateMap[EInputAction::Jump] = &Player::Jump;
	inputStateMap[EInputAction::Down] = &Player::Down;
	inputStateMap[EInputAction::Attack] = &Player::Attack;

	ipActionplayerStateMap[EInputAction::Left] = EPlayerState::IdleToRun;
	ipActionplayerStateMap[EInputAction::Right] = EPlayerState::IdleToRun;
	ipActionplayerStateMap[EInputAction::Jump] = EPlayerState::Jump;
	ipActionplayerStateMap[EInputAction::Down] = EPlayerState::Down;
	ipActionplayerStateMap[EInputAction::Attack] = EPlayerState::Attack;
	
	//PlayerStateInfo IdleToRun

	//playerStateFunctionMap[EPlayerState::IdleToRun] = 

}