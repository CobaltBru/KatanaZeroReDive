#include "Player.h"
#include "Collider.h"
#include "ImageManager.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"

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

	PlayerCollider = new Collider(this, EColliderType::Rect, {}, 30.0f, true, 1.0f);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider, ECollisionGroup::Player);

	// set player input key
	playerInput = new PlayerInput();
	playerInput->Init();

	// bind input action to state function
	BindState();

	velocity = FPOINT{ 0.1f, 0.1f };
	accel = FPOINT{ 0.0f, 0.0f };
	addaccel = FPOINT{ 0.01f, 0.01f };

	return S_OK;
}


void Player::Release()
{
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

	// fsm���� ������ �� switch���� �Ϲ���
	
	// get the pressed keys 
	std::vector<InputAction> actions = playerInput->GetActions();
	for (InputAction action : actions)
	{
		// action�� attack�� ��� func�� ��� �� ���ƾ���
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
			image = ImageManager::GetInstance()->FindImage("zeroidle");		// attack�� ���
		}
	}
}

void Player::MakeSnapShot(void* out)
{
}

void Player::BindState()
{
	inputStateMap[InputAction::Left] = &Player::Left;
	inputStateMap[InputAction::Right] = &Player::Right;
	inputStateMap[InputAction::Jump] = &Player::Jump;
	inputStateMap[InputAction::Down] = &Player::Down;

	inputStateMap[InputAction::Attack] = &Player::Attack;
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
	// frame ���������� pos �ٲ� �����
	// deltaTime ��� -> 1�ʿ� ���ļ� frame �ѹ��� �� �� �ֵ���
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
