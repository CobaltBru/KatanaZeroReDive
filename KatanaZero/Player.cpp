#include "Player.h"
#include "config.h"
#include "Collider.h"
#include "ImageManager.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "PlayerState.h"
#include "SpriteAnimator.h"
#include "PlayerAnim.h"
#include "RigidBody.h"
#include "AttackState.h"
#include "FallState.h"
#include "IdleState.h"
#include "RunState.h"
#include "JumpState.h"
#include "FlipState.h"
#include "WallSlideState.h"
#include "CommonFunction.h"

#include "SnapShotManager.h"


Player::Player()
{	
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	image = ImageManager::GetInstance()->FindImage("zeroidle");
	effectImage = nullptr;

	Pos = FPOINT{ 300.0f, 300.0f };
	switchTime = 0.02f;

	InitPlayerStates();
	state = states->Idle;

	InitPlayerInfo();

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 0.8f, 
		(float)image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() * 1.0f },
		true, 1.f);

	/*ObjectCollider = new Collider(this, EColliderType::Rect, {}, {
	(float)image->GetFrameWidth(),
	(float)image->GetFrameHeight()},
	true, 1.f);*/

	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectRigidBody = new RigidBody(this);
	InitRigidBody();

	InitScrollOffset();
	scrollSpeed = 300.f;

	// set player input key
	playerInput = new PlayerInput();
	//playerInput->Init();

	dir = EDirection::Right;

	//playerAnim = new PlayerAnim;
	//playerAnim->Init();
		
	bWall = false;

	return S_OK;
}


void Player::Release()
{
	if (playerInput)
	{		
		delete playerInput;
		playerInput = nullptr;
	}
	/*if (playerAnim)
	{
		playerAnim->Release();
		delete playerAnim;
		playerAnim = nullptr;
	}*/
	if (ObjectRigidBody)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
	if (states)
	{
		delete states->Attack;
		delete states->Fall;
		delete states->Idle;
		delete states->Run ;
		delete states->Jump ;
		delete states->Flip;
		delete states->WallSlide;
		delete states;
		states = nullptr;
	}
	if (info)
	{
		delete info;
		info = nullptr;
	}
}

void Player::Update()
{
	LastPos = Pos;
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
	
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	frameTimer += deltaTime;	

	// input
	PlayerState* newState = state->GetInput(this);	
	if (newState)
	{
		state = newState;
		state->Enter(this);
	}
	state->Update(this);

	// apply acceleration including gravity
	UpdateRigidBody();

	// collision
	UpdateCollision();

	// scroll offset
	Offset();
}

void Player::Render(HDC hdc)
{
	if (image != nullptr)
	{		
		if (dir == EDirection::Left)		
			image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, true,true,ScrollManager::GetInstance()->GetScale());	
		else		
			image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, false, true, ScrollManager::GetInstance()->GetScale());

		//if (dir == EDirection::Left)
		//	image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, true, true);
		//else
		//	image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, false, true);

		// update frame index
		if (frameTimer > switchTime)
		{
			FrameIndex++;
			frameTimer = 0.0f;
		}
		
		// init frame index
		if (FrameIndex >= image->GetMaxFrameX())
			FrameIndex %= image->GetMaxFrameX();	
	}

	if (effectImage != nullptr)
	{
		if (dir == EDirection::Left)
			effectImage->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, true, true, ScrollManager::GetInstance()->GetScale());
		else
			effectImage->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, false, true, ScrollManager::GetInstance()->GetScale());
		
		/*if (dir == EDirection::Left)
			effectImage->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, true, true);
		else
			effectImage->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, false, true);*/
	}
}

void Player::MakeSnapShot(void* out)
{
}

void Player::InitPlayerStates()
{
	states = new playerStates;
	states->Idle = new IdleState;
	states->Attack = new AttackState;
	states->Fall = new FallState;
	states->Run = new RunState;
	states->Jump = new JumpState;
	states->Flip = new FlipState;
	states->WallSlide = new WallSlideState;
}

void Player::InitPlayerInfo()
{
	info = new playerInfo;
	info->bIsAttack = false;
	info->bIsJump = false;
	info->bIsFlip = false;
}

void Player::InitBindState()
{
}

void Player::InitRigidBody()
{
	if (ObjectRigidBody == nullptr) return;

	ObjectRigidBody->SetElasticity(0.f);
	ObjectRigidBody->SetGravityVisible(true);
	ObjectRigidBody->SetAccelerationAlpha({ 0.f, 800.f });
	ObjectRigidBody->SetMass(10.f);
	//ObjectRigidBody->SetMass(5.f / ScrollManager::GetInstance()->GetScale() * ScrollManager::GetInstance()->GetScale());
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

	const float OffsetMinX = 200.f;
	const float OffsetMaxX = WINSIZE_X - 200.f;
	const float OffsetMinY = 100.f;
	const float OffsetMaxY = WINSIZE_Y - 100.f;

	const FPOINT scroll = ScrollManager::GetInstance()->GetScrollOffset();

	FPOINT newScroll{};
	if (OffsetMaxX < Pos.x + scroll.x)
		newScroll.x = -scrollSpeed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMinX > Pos.x + scroll.x && OffsetMinX < Pos.x)
		newScroll.x = scrollSpeed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMaxY < Pos.y + scroll.y)
		newScroll.y = -scrollSpeed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMinY > Pos.y + scroll.y && OffsetMinY < Pos.y)
		newScroll.y = scrollSpeed * TimerManager::GetInstance()->GetDeltaTime();

	ScrollManager::GetInstance()->SetScroll(newScroll);
}

void Player::UpdateRigidBody()
{
	ObjectRigidBody->Update();

	const FLineResult lineResult = ObjectRigidBody->GetResult();
	if (lineResult.LineType == ELineType::Wall)
	{
		if (lineResult.IsLeft) dir = EDirection::Left;

		if (ObjectRigidBody->IsGround() == false) 
			image = ImageManager::GetInstance()->FindImage("zerowallslide");
		else
		{
			image = ImageManager::GetInstance()->FindImage("zeroidle");
			state = states->Idle;
		}
		ObjectRigidBody->SetVelocity({ 0.f , 10.f });
		ObjectRigidBody->SetAccelerationAlpha({ 0.f , 500.f });
		bWall = true;
		bIsLeft = lineResult.IsLeft;
	}
	else
	{
		ObjectRigidBody->SetAccelerationAlpha({ 0.f , 800.f });
		bWall = false;
	}
}

void Player::UpdateCollision()
{
	FHitResult HitResult;
	
	// only checking the enemy
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);	// opponent

		// direction from player to enemy
		FPOINT PEDir;
		PEDir.x = HitResult.HitCollision->GetPos().x - ObjectCollider->GetPos().x;
		PEDir.y = HitResult.HitCollision->GetPos().y - ObjectCollider->GetPos().y;
		Normalize(PEDir);

		// knock enemy
		if (HitResult.HitCollision->GetOwner()->GetRigidBody())
			HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(PEDir * 400.f);
	}
}
