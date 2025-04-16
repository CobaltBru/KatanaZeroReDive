#include "Player.h"
#include "Collider.h"
#include "ImageManager.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RenderManager.h"

HRESULT Player::Init()
{
	image = ImageManager::GetInstance()->FindImage("zerowalk");
	Pos = FPOINT{ 300.0f, 300.0f };

	PlayerCollider = new Collider(this, EColliderType::Rect, {}, 30.0f, true, 1.0f);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider, ECollisionGroup::Player);

	playerInput = new PlayerInput();
	playerInput->Init();

	speed = 300.0f;

	keyActionMap = playerInput->GetkeyActionMap();

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
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Player::Render(HDC hdc)
{
	if (image != nullptr)
	{
		image->FrameRender(hdc, Pos.x, Pos.y, 0, 0);
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
}

void Player::Right()
{
}

void Player::Down()
{
}

void Player::Jump()
{
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
