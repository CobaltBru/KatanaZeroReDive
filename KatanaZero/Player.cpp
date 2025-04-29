#include "Player.h"
#include "config.h"
#include "Collider.h"
#include "ImageManager.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "RigidBody.h"
#include "CommonFunction.h"
#include "Bullet.h"
#include "SoundManager.h"


Player::Player()
{	
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	InitImage();

	

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 0.4f, 
		(float)image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() * 0.9f },
		false, 1.f);

	/*ObjectCollider = new Collider(this, EColliderType::Rect, {}, {
	(float)image->GetFrameWidth(),
	(float)image->GetFrameHeight()},
	true, 1.f);*/

	AttackCollider = new Collider(this, EColliderType::Sphere, {}, {
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 2.0f,
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 1.5f },
		false, 1.f);
	

	ObjectRigidBody = new RigidBody(this);
	InitRigidBody();


	InitScrollOffset();
	scrollSpeed = 300.f;

	

	return S_OK;
}



void Player::Release()
{
	
	if (ObjectRigidBody)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
	
}

void Player::Update()
{
	LastPos = Pos;
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
	
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SHIFT))	
	{
		
		// slow motion
		//슬로우
		// GetDeltaTime 인자에 false 넣으면 오리지날 DeltaTime가져오고 true넣으면 슬로우 계수 붙은 DeltaTime가져옵니다  디폴트 true임
		// TimerManager::GetInstance()->GetDeltaTime();

		// 여기서 안됨
		//슬로우 주기                  //슬로우계수 0 ~ 1 / 해당 계수까지 가는데 몇초동안 보간할거냐
		//TimerManager::GetInstance()->SetSlow(0.1f, 0.2f);
		
	}
	else if (KeyManager::GetInstance()->IsOnceKeyUp(VK_SHIFT))
	{
		
		// 슬로우 풀기
		//TimerManager::GetInstance()->SetSlow(1.f, 0.2f);
	}

	
	// apply acceleration including gravity
	UpdateRigidBody();

	// collision
	
	// scroll offset
	Offset();

}

void Player::Render(HDC hdc)
{
	

}



void Player::InitRigidBody()
{
	if (ObjectRigidBody == nullptr) return;

	ObjectRigidBody->SetElasticity(0.f);
	ObjectRigidBody->SetGravityVisible(true);
	ObjectRigidBody->SetAccelerationAlpha({ 0.f, 800.f });
	ObjectRigidBody->SetMass(10.f);
	ObjectRigidBody->SetMaxVelocity({ 600.f, 1000.f });
	ObjectRigidBody->SetFriction(600.f);
}

void Player::InitScrollOffset()
{
	ScrollManager::GetInstance()->SetFocus(true);
	FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
	while (true)
	{
		Offset();

		const FPOINT newScroll = ScrollManager::GetInstance()->GetScroll();

		if (scroll.x != newScroll.x || scroll.y != newScroll.y)
			scroll = newScroll;
		else
			break;
	}
}

void Player::Offset()
{
	if (!ScrollManager::GetInstance()->IsFocus()) return;

	float newScrollSpeed = max(ObjectRigidBody->GetVelocity().x, ObjectRigidBody->GetVelocity().y);
	scrollSpeed = newScrollSpeed;

	const FPOINT scroll = ScrollManager::GetInstance()->GetScrollOffset();

	
}

void Player::UpdateRigidBody()
{
	ObjectRigidBody->Update();

}




void Player::InitImage()
{
	
}

