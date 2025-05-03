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

Player::Player()
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
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);
	ObjectCollider->SetPos(Pos);

	ObjectRigidBody = new RigidBody(this);
	InitRigidBody();
	

	InitScrollOffset();
	scrollSpeed = 300.f;

	

	StateInit();
	currentState = STATE::IDLE;
	changeState(currentState);
	InitAnimator();
	animator->startAnimation("idle");
	

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

}

void Player::Update()
{
	LastPos = Pos;
	
	
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SHIFT))	
	{
		isEffect = true;
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
		isEffect = false;
		// 슬로우 풀기
		//TimerManager::GetInstance()->SetSlow(1.f, 0.2f);
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
	{
		if(currentState != STATE::ATTACK)
			changeState(STATE::ATTACK);
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

	stateMachine[(int)currentState]->Update();
	animator->Update(Pos, way > 0 ? false : true, isEffect);
	// apply acceleration including gravity
	UpdateRigidBody();

	// collision
	
	// scroll offset
	Offset();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Player::Render(HDC hdc)
{
	/*if (image != nullptr)
	{
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		image->FrameRender(hdc, Pos.x + ScrollManager::GetInstance()->GetScroll().x, Pos.y + ScrollManager::GetInstance()->GetScroll().y, 0, 0, false, true, ScrollManager::GetInstance()->GetScale());
	}*/
	animator->Render(hdc);
	WCHAR tch[5];
	wsprintfW(tch, L"%d\0", (int)currentState);
	TextOutW(hdc, Pos.x, Pos.y - 70.f, tch, lstrlen(tch));
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


