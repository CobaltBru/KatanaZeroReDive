#include "Player.h"
#include "config.h"
#include "Collider.h"
#include "ImageManager.h"
#include "Image.h"
#include "GPImage.h"
#include "GPImageManager.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "LineManager.h"
#include "RigidBody.h"
#include "CommonFunction.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "PlayerStateHeaders.h"
#include "SpriteAnimation.h"
#include "Animator.h"

#include "ArrowUI.h"
#include "PickUpHand.h"
#include "PickUp.h"
#include "UIGame.h"

Player::Player(): ScrollSpeed(0.f), RightHand(nullptr), UIGameObj(nullptr), ArrowUIObj(nullptr)
{	
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	InitImage();
	Pos = { 250.f,200.f };
	way = 1;
	isEffect = false;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(), 
		(float)image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() - 20.f },
		true, 1.f);
	AttackCollider = new Collider(this, EColliderType::Sphere, {}, {
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 2.0f,
		(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale() * 1.5f },
		true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Player);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);
	ObjectCollider->SetPos(Pos);

	ObjectRigidBody = new RigidBody(this);
	InitRigidBody();
	

	InitScrollOffset();
	scrollSpeed = 600.f;;

	RightHand = new PickUpHand(this);

	StateInit();
	currentState = STATE::IDLE;
	changeState(currentState);
	InitAnimator();
	animator->startAnimation("idle");
	canUseSkill1 = true;
	skill1On = false;
	skillDistance = 500.f;
	skillVec = { 0.f,0.f };
	skill2On = false;
	currentLen = 0.f;
	skill2Timer = 0.f;
	return S_OK;
}

HRESULT Player::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImage();
	Pos = InPos;
	way = InFlip ? -1 : 1;
	isEffect = false;
	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	AttackCollider = new Collider(this, EColliderType::Sphere, {}, {
		30.f * ScrollManager::GetInstance()->GetScale() * 2.0f,
		30.f * ScrollManager::GetInstance()->GetScale() * 2.0f },
		true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Player);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);
	ObjectCollider->SetPos(Pos);

	ObjectRigidBody = new RigidBody(this);
	InitRigidBody();


	InitScrollOffset();
	scrollSpeed = 600.f;

	RightHand = new PickUpHand(this);

	StateInit();
	currentState = STATE::IDLE;
	changeState(currentState);
	InitAnimator();
	animator->startAnimation("idle");
	canUseSkill1 = true;
	skill1On = false;
	skillDistance = 500.f;
	skillVec = { 0.f,0.f };
	skill2On = false;
	currentLen = 0.f;
	skill2Timer = 0.f;
	return S_OK;
}



void Player::Release()
{
	
	if (ObjectRigidBody)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
	for (int i = 0; i < (int)STATE::END; i++)
	{
		if (stateMachine[i])
		{
			delete stateMachine[i];
			stateMachine[i] = nullptr;
		}
	}
	stateMachine.clear();
	
	if (animator != nullptr)
	{
		animator->Release();
		delete animator;
		animator = nullptr;
	}
	if (RightHand != nullptr)
	{
		delete RightHand;
		RightHand = nullptr;
	}
}

void Player::Update()
{
	LastPos = Pos;
	
	
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_SHIFT))
	{
		if (canUseSkill1)
		{
			if (UIGameObj->getBattery() > 0.0001f)
			{
				skill1On = true;
				isEffect = true;
				UIGameObj->UpdateSlow(true);
				//슬로우 주기                  //슬로우계수 0 ~ 1 / 해당 계수까지 가는데 몇초동안 보간할거냐
				TimerManager::GetInstance()->SetSlow(0.1f, 0.2f);
			}
			else
			{
				skill1On = false;
				canUseSkill1 = false;
				isEffect = false;
				UIGameObj->UpdateSlow(false);
				TimerManager::GetInstance()->SetSlow(1.f, 0.2f);
			}
		}
		
		
	}
	else
	{
		skill1On = false;
		canUseSkill1 = true;
		isEffect = false;
		UIGameObj->UpdateSlow(false);
		TimerManager::GetInstance()->SetSlow(1.f, 0.2f);
	}



	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
	{
		FPOINT attackVec = { 0.f,0.f };
		attackVec.x = g_ptMouse.x - (Pos.x + ScrollManager::GetInstance()->GetScroll().x);
		attackVec.y = g_ptMouse.y - (Pos.y + ScrollManager::GetInstance()->GetScroll().y);
		Normalize(attackVec);
		AttackCollider->SetPivot(attackVec * 60.f);
		if(currentState != STATE::ATTACK)
			changeState(STATE::ATTACK);
		if (skill2On)
		{
			SetPos({ Pos.x  + skillVec.x ,
				Pos.y + skillVec.y });
			skill2On = false;
			SoundManager::GetInstance()->PlaySounds("slowoff", EChannelType::Effect);
			SoundManager::GetInstance()->PitchOrigin(EChannelType::BGM);
			TimerManager::GetInstance()->SetSlow(1.f, 0.1f);
		}
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('K'))
	{
		if (currentState != STATE::DEAD)
		{
			ObjectCollider->SetSize({ ScrollManager::GetInstance()->GetScale() * 41,
				ScrollManager::GetInstance()->GetScale() * 29 });
			changeState(STATE::DEAD);
		}
			
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_CONTROL))
	{
		skill2On = true;
		currentLen = 0.f;
		skill2Timer = 0.f;
		SoundManager::GetInstance()->PlaySounds("slowon", EChannelType::Effect);
		SoundManager::GetInstance()->PitchDown(EChannelType::BGM);
		
	}
	if (KeyManager::GetInstance()->IsOnceKeyUp(VK_CONTROL))
	{
		skill2On = false;
		SoundManager::GetInstance()->PlaySounds("slowoff", EChannelType::Effect);
		SoundManager::GetInstance()->PitchOrigin(EChannelType::BGM);
	}
	if (skill2On)
	{
		TimerManager::GetInstance()->SetSlow(0.01f, 0.1f);
		dragonSkillUpdate();
	}
	else
	{
		if(skill1On ==false)
			TimerManager::GetInstance()->SetSlow(1.f, 0.1f);
	}
	stateMachine[(int)currentState]->Update();
	animator->Update({Pos.x + ScrollManager::GetInstance()->GetScroll().x,
		Pos.y + ScrollManager::GetInstance()->GetScroll().y}, bFlip, isEffect);
	// apply acceleration including gravity
	UpdateRigidBody();
	PickUpUpdate();
	// collision
	UpdateCollision();
	// scroll offset
	Offset();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Player::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	/*if (image != nullptr)
	{
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		image->FrameRender(hdc, Pos.x + ScrollManager::GetInstance()->GetScroll().x, Pos.y + ScrollManager::GetInstance()->GetScroll().y, 0, 0, false, true, ScrollManager::GetInstance()->GetScale());
	}*/
	animator->Render(hdc);
	if (skill2On)
	{
		dragonSkillRender(hdc);
	}
	WCHAR tch[5];
	wsprintfW(tch, L"%d\0", (int)currentState);
	TextOutW(hdc, Pos.x + Scroll.x, Pos.y - 70.f + Scroll.y, tch, lstrlen(tch));
}

void Player::InitImage()
{
	image = ImageManager::GetInstance()->FindImage("spr_beer_bottle_3_0");
}

void Player::InitAnimator()
{
	animator = new Animator();
	SpriteAnimation* tmpAnimation;
	GPImage* tmpImage;

	//idle
	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_idle");
	tmpAnimation->Init(tmpImage, 0.08f, true);
	animator->pushAnimation("idle", tmpAnimation);

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_run");
	tmpAnimation->Init(tmpImage, 0.08f, true);
	animator->pushAnimation("run", tmpAnimation);

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_idle_to_run");
	tmpAnimation->Init(tmpImage, 0.1f, false);
	animator->pushAnimation("idle_to_run", tmpAnimation, "run");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_run_to_idle");
	tmpAnimation->Init(tmpImage, 0.1f, false);
	animator->pushAnimation("run_to_idle", tmpAnimation, "idle");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_jump");
	tmpAnimation->Init(tmpImage, 0.08f, true);
	animator->pushAnimation("jump", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_fall");
	tmpAnimation->Init(tmpImage, 0.08f, true);
	animator->pushAnimation("fall", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_roll");
	tmpAnimation->Init(tmpImage, 0.04f, false);
	animator->pushAnimation("roll", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_crouch");
	tmpAnimation->Init(tmpImage, 0.1f, false);
	animator->pushAnimation("crouch", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_wallslide");
	tmpAnimation->Init(tmpImage, 0.1f, false);
	animator->pushAnimation("wall", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_attack");
	tmpAnimation->Init(tmpImage, 0.05f, false);
	animator->pushAnimation("attack", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_hurtfly_begin");
	tmpAnimation->Init(tmpImage, 0.08f, false);
	animator->pushAnimation("hurtfly_begin", tmpAnimation, "hurtfly");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_hurtfly");
	tmpAnimation->Init(tmpImage, 0.08f, true);
	animator->pushAnimation("hurtfly", tmpAnimation, "NONE");

	tmpAnimation = new SpriteAnimation();
	tmpImage = GPImageManager::GetInstance()->FindImage("dragon_hurtground");
	tmpAnimation->Init(tmpImage, 0.12f, false);
	animator->pushAnimation("hurtground", tmpAnimation, "NONE");

	skillImage = GPImageManager::GetInstance()->FindImage("dragon_attack");
}



void Player::InitRigidBody()
{
	if (ObjectRigidBody == nullptr) return;

	ObjectRigidBody->SetElasticity(0.f);
	ObjectRigidBody->SetGravityVisible(true);
	ObjectRigidBody->SetAccelerationAlpha({ 0.f, 1500.f });
	ObjectRigidBody->SetMass(5.f);
	ObjectRigidBody->SetMaxVelocity({ 800.f, 1000.f });
	ObjectRigidBody->SetFriction(300.f);
}

void Player::UpdateRigidBody()
{
	ObjectRigidBody->Update();
	FPOINT currentV = ObjectRigidBody->GetVelocity();
	/*if (abs(currentV.x) < 5.f)
	{
		ObjectRigidBody->SetVelocity({ 0,currentV.y });
	}*/
}

void Player::InitScrollOffset()
{
	//포커스해야 오프셋할 수 있듬.
	ScrollManager::GetInstance()->SetFocus(true);

	FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	while (true)
	{
		Offset();

		const FPOINT NewScroll = ScrollManager::GetInstance()->GetScroll();

		if (Scroll.x != NewScroll.x || Scroll.y != NewScroll.y)
			Scroll = NewScroll;
		else
			break;
	}
}

void Player::Offset()
{
	if (!ScrollManager::GetInstance()->IsFocus()) return;

	/*float newScrollSpeed = max(ObjectRigidBody->GetVelocity().x, ObjectRigidBody->GetVelocity().y);
	scrollSpeed = newScrollSpeed;*/

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

void Player::StateInit()
{
	stateMachine.resize((int)STATE::END);

	PlayerIdleAction* playerIdle = new PlayerIdleAction(this);
	stateMachine[(int)STATE::IDLE] = playerIdle;

	PlayerWalkAction* playerWalk = new PlayerWalkAction(this);
	stateMachine[(int)STATE::WALK] = playerWalk;

	PlayerLowAction* playerLow = new PlayerLowAction(this);
	stateMachine[(int)STATE::LOW] = playerLow;

	PlayerAirAction* playerAir = new PlayerAirAction(this);
	stateMachine[(int)STATE::AIR] = playerAir;

	PlayerWallAction* playerWall = new PlayerWallAction(this);
	stateMachine[(int)STATE::WALL] = playerWall;

	PlayerAttackAction* playerAttack = new PlayerAttackAction(this);
	stateMachine[(int)STATE::ATTACK] = playerAttack;

	PlayerRollAction* playerRoll = new PlayerRollAction(this);
	stateMachine[(int)STATE::ROLL] = playerRoll;

	PlayerDeadAction* playerDead = new PlayerDeadAction(this);
	stateMachine[(int)STATE::DEAD] = playerDead;

	
	
	
}

void Player::changeState(STATE state)
{
	//ObjectRigidBody->SetDown(false);
	stateMachine[(int)currentState]->onExit();
	oldState = currentState;
	currentState = state;
	stateMachine[(int)currentState]->onEnter();
}

string Player::stateToString()
{
	switch (currentState)
	{
	case STATE::IDLE:
		return "IDLE";
		break;
	case STATE::WALK:
		return "WALK";
		break;
	case STATE::LOW:
		return "LOW";
		break;
	case STATE::AIR:
		return "AIR";
		break;
	case STATE::WALL:
		return "WALL";
		break;
	case STATE::ATTACK:
		return "ATTACK";
		break;
	case STATE::ROLL:
		return "ROLL";
		break;
	case STATE::DEAD:
		return "DEAD";
		break;
	}
}

void Player::PickUpUpdate()
{
	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Item))
	{
		if (ArrowUIObj != nullptr)
		{
			ArrowUIObj->SetPos({ HitResult.HitCollision->GetOwner()->GetPos().x,HitResult.HitCollision->GetOwner()->GetPos().y - 80.f });
			ArrowUIObj->SetVisible(true);
		}


		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
		{
			if (RightHand != nullptr)
			{
				if (RightHand->GetPickUpItem() == nullptr)
				{
					RightHand->SetPickUpItem(static_cast<PickUp*>(HitResult.HitCollision->GetOwner()), FPOINT{ 0.f,(float)-animator->getGPImage()->getFrameHeight()});

					if (UIGameObj != nullptr)
						UIGameObj->SetRightItem(RightHand->GetPickUpItem()->GetImageKey(), { 12.f,-4.f }, 0, RightHand->GetPickUpItem()->GetScale() * ScrollManager::GetInstance()->GetScale());
					SoundManager::GetInstance()->PlaySounds("sound_player_grabtea", EChannelType::Effect);
				}
				else
					Shoot();
			}
		}
	}
	else
	{
		if (ArrowUIObj != nullptr)
			ArrowUIObj->SetVisible(false);
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
		Shoot();
}

void Player::Shoot()
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	float Radian = atan2f((Pos.y + Scroll.y) - g_ptMouse.y, (Pos.x + Scroll.x) - g_ptMouse.x);
	float Angle = (Radian * 180.f / 3.14) + 180.f;
	RightHand->Shoot(Pos, Angle, 5.f);

	if (UIGameObj != nullptr)
		UIGameObj->SetRightItem("", {}, 0, 1);
}

void Player::UpdateCollision()
{
	FHitResult HitResult;
	if (currentState == STATE::ROLL) return;
	if ((currentState != STATE::ATTACK) && CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy) ||
		(currentState != STATE::ATTACK) && CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Bullet))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);	// opponent

		// direction from player to enemy
		FPOINT PEDir = HitResult.HitCollision->GetPos() - ObjectCollider->GetPos();
		ObjectCollider->SetSize({ ScrollManager::GetInstance()->GetScale() * 41,
				ScrollManager::GetInstance()->GetScale() * 29 });
		changeState(STATE::DEAD);

		SoundManager::GetInstance()->PlaySounds("zerodie", EChannelType::Effect);
	}

	// player attack enemy
	if ((currentState == STATE::ATTACK) && CollisionManager::GetInstance()->CollisionAABB(AttackCollider, HitResult, ECollisionGroup::Enemy))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);	// opponent

		FPOINT PEDir = HitResult.HitCollision->GetPos() - ObjectCollider->GetPos();

		// knock enemy
		if (HitResult.HitCollision->GetOwner()->GetRigidBody())
			HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(PEDir * 400.f);
	}

	// player attack bullet
	if ((currentState == STATE::ATTACK) && CollisionManager::GetInstance()->CollisionAABB(AttackCollider, HitResult, ECollisionGroup::Bullet))
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

		SoundManager::GetInstance()->PlaySounds("zeroslicebullet", EChannelType::Effect);
	}
}

void Player::dragonSkillUpdate()
{
	FPOINT attackVec = { 0.f,0.f };
	skill2Timer += TimerManager::GetInstance()->GetDeltaTime(false);
	attackVec.x = g_ptMouse.x - (Pos.x + ScrollManager::GetInstance()->GetScroll().x);
	attackVec.y = g_ptMouse.y - (Pos.y + ScrollManager::GetInstance()->GetScroll().y);
	float fxAngle = atan2f(attackVec.y, attackVec.x) * (180.f / 3.14159265f);
	float len = GetLength(attackVec);
	Normalize(attackVec);
	len = min(len, skillDistance);
	float percent = skill2Timer / 0.5f;
	currentLen = skillDistance * percent;
	if (currentLen > len) currentLen = len;
	skillVec = attackVec * currentLen;
}

void Player::dragonSkillRender(HDC hdc)
{
	Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);

	
	HPEN hPen1 = CreatePen(PS_SOLID, 4, RGB(0, 206, 209));
	HPEN hPen2 = CreatePen(PS_SOLID, 20, RGB(0, 255, 255));
	FPOINT scroll = ScrollManager::GetInstance()->GetScroll();

	HPEN oldpen = (HPEN)SelectObject(hdc, hPen1);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, Pos.x - skillDistance + scroll.x, Pos.y - skillDistance + scroll.y,
		Pos.x + skillDistance + scroll.x, Pos.y + skillDistance + scroll.y);
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hPen2);
	MoveToEx(hdc, Pos.x + scroll.x, Pos.y + scroll.y,nullptr);
	LineTo(hdc, Pos.x + scroll.x + skillVec.x , Pos.y + scroll.y + skillVec.y);
	SetFlip(skillVec.x > 0 ? false : true);
	way = skillVec.x > 0 ? 1 : -1;
	skillImage->Middle_RenderFrameScale(pGraphics, { Pos.x + scroll.x + skillVec.x , Pos.y + scroll.y + skillVec.y }
	,0, bFlip, 0.5f,ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
	SelectObject(hdc, oldpen);
	DeleteObject(hPen1);
	DeleteObject(hPen2);

	delete pGraphics;
}


