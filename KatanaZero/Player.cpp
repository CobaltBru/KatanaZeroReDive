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
	image = ImageManager::GetInstance()->FindImage("zerowalk");
	Pos = FPOINT{ 300.0f, 300.0f };

	// TODO
	FrameIndexMax = 10;

	PlayerCollider = new Collider(this, EColliderType::Rect, {}, 30.0f, true, 1.0f);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider, ECollisionGroup::Player);

	// set player input key
	playerInput = new PlayerInput();
	playerInput->Init();

	// bind input action to state function
	BindState();

	speed = 0.1f;

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
	// get the pressed keys 
	std::vector<InputAction> actions = playerInput->GetActions();
	for (InputAction action : actions)
	{
		stateFunction func = inputStateMap[action];
		func(*this);
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Player::Render(HDC hdc)
{
	if (image != nullptr)
	{
		image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0);

		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		//image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, FrameIndex, 0);

		FrameIndex++;
		if (FrameIndex >= FrameIndexMax) FrameIndex %= FrameIndexMax;
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
	Pos.x -= speed;
}

void Player::Right()
{
	Pos.x += speed;
}

void Player::Down()
{
	Pos.y += speed;
}

void Player::Jump()
{
	Pos.y -= speed;
}

void Player::Fall()
{
}

void Player::Attack()
{
}

void Player::Dead()
{
}

void Player::WallSlide()
{
}
