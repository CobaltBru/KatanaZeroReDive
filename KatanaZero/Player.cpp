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
#include "Bullet.h"
#include "EffectManager.h"

#include "SnapShotManager.h"


Player::Player()
{	
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	InitImage();

	image = ImageManager::GetInstance()->FindImage("zeroidle");
	effectImage = nullptr;

	Pos = FPOINT{ 1200, 700 };
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

	AttackCollider = new Collider(this, EColliderType::Sphere, {}, {
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 1.5f,
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 1.5f },
		false, 1.f);
	
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Player);

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
		
	info->bIsWall = false;

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
	attackTimer += deltaTime;

	if (attackTimer > info->attackCoolTime)
	{
		info->bCanAttack = true;
	 }

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SHIFT))	
	{
		info->bIsShift = true;
		info->bIsShiftChanged = true;
		// change image, slow motion
	}
	else if (KeyManager::GetInstance()->IsOnceKeyUp(VK_SHIFT))
	{
		info->bIsShift = false;
		info->bIsShiftChanged = true;
	}

	// input
	PlayerState* newState = state->GetInput(this);	
	if (newState)
	{
		state = newState;
		state->Enter(this);
	}
	// update state
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
		image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, (dir == EDirection::Left ? true : false), true, ScrollManager::GetInstance()->GetScale());

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

	if (info->bIsAttack)
		EffectManager::GetInstance()->Activefx("normalslash", GetPos() + FPOINT{-30.f, -30.f}, 0.f, (dir == EDirection::Left ? true : false));
		
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
	info->bIsShift = false;
	info->bIsShiftChanged = false;
	info->bIsWall = false;
	info->attackCoolTime = .7f;
	info->prevState = "";
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

	//const FLineResult lineResult = ObjectRigidBody->GetResult();
	//if (lineResult.LineType == ELineType::Wall)
	//{
	//	if (lineResult.IsLeft) dir = EDirection::Left;

	//	ObjectRigidBody->SetVelocity({ 0.f , 10.f });
	//	ObjectRigidBody->SetAccelerationAlpha({ 0.f , 500.f });
	//	info->bIsWall = true;
	//	bIsLeft = lineResult.IsLeft;
	//}
	//else
	//{
	//	ObjectRigidBody->SetAccelerationAlpha({ 0.f , 800.f });
	//	info->bIsWall = false;
	//}
}

void Player::UpdateCollision()
{
	FHitResult HitResult;
	
	if (info->bIsFlip) return;

	// player die
	if (!info->bIsAttack && CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy) ||
		!info->bIsAttack && CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Bullet))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);	// opponent

		// direction from player to enemy
		FPOINT PEDir = HitResult.HitCollision->GetPos() - ObjectCollider->GetPos();

		//// die
		//if (HitResult.HitCollision->GetOwner()->GetRigidBody())
		//	ObjectRigidBody->AddVelocity(-PEDir * 100.f);
	}

	// player attack enemy
	if (info->bIsAttack && CollisionManager::GetInstance()->CollisionAABB(AttackCollider, HitResult, ECollisionGroup::Enemy))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);	// opponent

		FPOINT PEDir = HitResult.HitCollision->GetPos() - ObjectCollider->GetPos();

		// knock enemy
		if (HitResult.HitCollision->GetOwner()->GetRigidBody())
			HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(PEDir * 400.f);
	}

	// player attack bullet
	if (info->bIsAttack && CollisionManager::GetInstance()->CollisionAABB(AttackCollider, HitResult, ECollisionGroup::Bullet))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);	// opponent

		FPOINT PEDir = HitResult.HitCollision->GetPos() - ObjectCollider->GetPos();

		// knock enemy
		if (HitResult.HitCollision->GetOwner())
		{			
			BulletTest* bullet = dynamic_cast<BulletTest*>(HitResult.HitCollision->GetOwner());
			if (bullet)
			{
				/*float angle;
				if (dir == EDirection::Right) angle = -30.f;
				else angle = -150.f;*/
				float angle = bullet->GetAngle() + 180.f;
				if (angle >= 360.f) angle -= 360.f;
				bullet->SetAngle(angle);
			}
		}
			
	}
}


void Player::InitImage()
{
	//zero
	ImageManager::GetInstance()->AddImage("zeroidle", L"Image/zero_idle.bmp", 420, 39, 11, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zerojump", L"Image/zero_jump.bmp", 136, 44, 4, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zerorun", L"Image/zero_run.bmp", 460, 34, 10, 1, true, RGB(255, 255, 255));
	//ImageManager::GetInstance()->AddImage("zeroflip", L"Image/zero_roll.bmp", 350, 33, 7, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zeroflip", L"Image/zero_flip.bmp", 574, 46,11, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zerofall", L"Image/zero_fall.bmp", 176, 50, 4, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zerocrouch", L"Image/zero_crouch.bmp", 36, 40, 1, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zeroattack", L"Image/zero_attack.bmp", 448, 44, 7, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zerodrawsword", L"Image/zero_drawsword.bmp", 1843, 61, 19, 1, true, RGB(255, 255, 255));		
	ImageManager::GetInstance()->AddImage("zerowallslide", L"Image/zero_wallslide.bmp", 37, 42, 1, 1, true, RGB(255, 255, 255));
	ImageManager::GetInstance()->AddImage("zeroidletorun", L"Image/zero_idle_to_run.bmp", 184, 34, 4, 1, true, RGB(255, 255, 255));	

	// shadow
	ImageManager::GetInstance()->AddImage("zeroidleshadow", L"Image/zero_idle_shadow.bmp", 420, 39, 11, 1, true, RGB(255, 0, 255));
	//ImageManager::GetInstance()->AddImage("zerojumpshadow", L"Image/zero_jump_shadow.bmp", 136, 44, 4, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("zerorunshadow", L"Image/zero_run_shadow.bmp", 440, 32, 10, 1, true, RGB(255, 0, 255));
	//ImageManager::GetInstance()->AddImage("zeroflipshadow", L"Image/zero_roll_shadow.bmp", 329, 31, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("zeroflipshadow", L"Image/zero_flip_shadow.bmp", 528, 44, 11, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("zerofallshadow", L"Image/zero_fall_shadow.bmp", 164, 49, 4, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("zeroattackshadow", L"Image/zero_attack_shadow.bmp", 420, 41, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("zerowallslideshadow", L"Image/zero_wallslide_shadow.bmp", 38, 42, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("zeroidletorunshadow", L"Image/zero_idle_to_run_shadow.bmp", 184, 34, 4, 1, true, RGB(255, 0, 255));

	// slash
	ImageManager::GetInstance()->AddImage("normalslash", L"Image/fx/NormalSlash.bmp", 530, 32, 5, 1, true, RGB(255, 255, 255));
}




//if (effectImage != nullptr && info->bIsAttack)
	//{
	//	FPOINT attackDir = { 0.f, 0.f };
	//	attackDir.x = g_ptMouse.x - Pos.x;
	//	attackDir.y = g_ptMouse.y - Pos.y;
	//	Normalize(attackDir);
	//	AttackCollider->SetPivot({ attackDir.x * 70.f,attackDir.y * 70.f });
	//	if (dir == EDirection::Left)
	//		effectImage->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, true, true, ScrollManager::GetInstance()->GetScale());
	//	else
	//		effectImage->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, false, true, ScrollManager::GetInstance()->GetScale());
	//}

