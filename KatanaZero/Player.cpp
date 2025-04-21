#include "Player.h"
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
	switchTime = 0.02f;

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.0f, true, 1.0f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectRigidBody = new RigidBody(this);
	InitRigidBody();

	InitScrollOffset();
	scrollSpeed = 300.f;

	// set player input key
	playerInput = new PlayerInput();
	playerInput->Init();
	dir = EDirection::Right;

	playerAnim = new PlayerAnim;
	playerAnim->Init();
	
	bWall = false;

	// bind input action to state function
	InitBindState();

	currPlayerState = EPlayerState::Idle;
	playerAnimFunc = IdleAnimFunc;
	prevPlayerState = EPlayerState::Idle;

	return S_OK;
}


void Player::Release()
{
	if (playerInput)
	{		
		delete playerInput;
		playerInput = nullptr;
	}
	if (playerAnim)
	{
		playerAnim->Release();
		delete playerAnim;
		playerAnim = nullptr;
	}
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

	playerInput->UpdateKeystate();
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	frameTimer += deltaTime;	

	// get input
	// released
	std::vector<EInputAction> released = playerInput->GetReleased();
	for (EInputAction action : released)
	{
		if (currPlayerState == ipActionPlayerStateMap[action])
		{
			currPlayerState = EPlayerState::Idle;			
		}
	}

	// held
	std::vector<EInputAction> held = playerInput->GetHeld();
	for (EInputAction action : held)
	{

		switch (action)
		{
		case EInputAction::Jump:
			currPlayerState = EPlayerState::Idle;
			break;
		case EInputAction::Left:
			dir = EDirection::Left;
			currPlayerState = EPlayerState::IdleToRun;
			break;
		case EInputAction::Right:
			dir = EDirection::Right;
			currPlayerState = EPlayerState::IdleToRun;
			break;
		case EInputAction::Down:			
			if (prevPlayerState == EPlayerState::Idle)
			{
				currPlayerState = EPlayerState::Crouch;
			}
			break;
		}
	}

	// pressed
	std::vector<EInputAction> pressed = playerInput->GetPressed();

	// update by switchTime
	//if (frameTimer < switchTime) return;
	//frameTimer -= switchTime;

	// prev state 저장
	prevPlayerState = currPlayerState;

	// switch player state
	for (EInputAction action : pressed)
	{		
		if (prevPlayerState == EPlayerState::Attack) break;

		switch (action)
		{
		case EInputAction::Left:
			dir = EDirection::Left;
			currPlayerState = EPlayerState::IdleToRun;
			break;
		case EInputAction::Right:
			dir = EDirection::Right;
			currPlayerState = EPlayerState::IdleToRun;
			break;
		case EInputAction::Jump:
			currPlayerState = EPlayerState::Jump;
			break;
		case EInputAction::Down:
			if (prevPlayerState == EPlayerState::Idle)
			{
				// TODO 시간 프레임만큼 유지해야함
				currPlayerState = EPlayerState::Crouch;
			}
			else
			{ 
				currPlayerState = EPlayerState::Flip;
			}
			break;
		case EInputAction::Attack:
			currPlayerState = EPlayerState::Attack;
			break;
		}
	}

	// player state
	// update animation and function when player is in new state
	if (prevPlayerState != currPlayerState)
	{
		auto it = playerStateFunctionMap.find(currPlayerState);
		if (it != playerStateFunctionMap.end())
		{
			playerAnimFunc = it->second;
			image = playerAnimFunc.animator->GetImage();
			playerAnimFunc.animator->InitFrame();

			playerAnimFunc.func(*this, dir);
		}
	}
	else		// existing state
	{
		image = playerAnimFunc.animator->GetImage();

		// update frame index
		if (playerAnimFunc.animator->UpdateFrame(deltaTime) == false)
		{
			playerAnimFunc = IdleAnimFunc;
		}

		// update movement
		playerAnimFunc.func(*this, dir);
	}

	// apply acceleration including gravity
	ObjectRigidBody->Update();

	const FLineResult lineResult = ObjectRigidBody->GetResult();
	if (lineResult.LineType == ELineType::Wall)
	{
		ObjectRigidBody->SetAccelerationAlpha({ 0.f , 500.f });
		bWall = true;
		bIsLeft = lineResult.IsLeft;
	}
	else
	{
		ObjectRigidBody->SetAccelerationAlpha({ 0.f , 800.f });
		bWall = false;
	}

	// collision

	// scroll offset
	Offset();
}

	//for (EInputAction action : pressed)
	//{
	//	if (action == EInputAction::Left) dir = EDirection::Left;
	//	newPlayerStates.push_back(ipActionPlayerStateMap[action]);
	//}

	//// update player state
	//if (!newPlayerStates.empty())
	//{
	//	prevPlayerState = currPlayerState;
	//	ChangeState(currPlayerState, newPlayerStates);
	//	newPlayerStates.clear();
	//}
	
	
	//// action이 attack인 경우 func이 몇번 더 돌아야함
	//stateFunction func = inputStateMap[action];
	//func(*this);

void Player::Render(HDC hdc)
{
	if (image != nullptr)
	{		

		int FrameIndexMax = image->GetMaxFrameX();
		if (dir == EDirection::Left)
		{
			image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0, true);
		}
		else
		{
			image->FrameRender(hdc, Pos.x, Pos.y, FrameIndex, 0);
		}

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

void Player::Idle(EDirection dir)
{
}

void Player::Run(EDirection dir)
{
	if (dir == EDirection::Right)
	{
		ObjectRigidBody->AddVelocity({200.f, 0.f });
	}
	else
	{
		ObjectRigidBody->AddVelocity({ -200.f , 0.f });
	}

	//if (std::abs(velocity.x) > playerAnim->GetIdleToRunSpeed())
	currPlayerState = EPlayerState::Run;
}

void Player::Walk(EDirection dir)
{
}

void Player::Flip(EDirection dir)
{
	if (dir == EDirection::Right)
	{
	}
	else
	{		
	}
}

void Player::Down(EDirection dir)
{
	ObjectRigidBody->SetDown(true);
}

void Player::Jump(EDirection dir)
{
	ObjectRigidBody->AddVelocity({ 0.f, -2.f });
}

void Player::Fall(EDirection dir)
{
}

void Player::Attack(EDirection dir)
{
	// frame 끝날때까지 pos 바꿔 줘야함
	// deltaTime 고려 -> 1초에 걸쳐서 frame 한바퀴 돌 수 있도록
	/*velocity.x += 0.001f;
	velocity.y += 0.001f;*/
}

void Player::WallSlide(EDirection dir)
{
}

void Player::InitBindState()
{
	inputStateMap[EInputAction::Jump] = &Player::Jump;
	inputStateMap[EInputAction::Down] = &Player::Down;
	inputStateMap[EInputAction::Attack] = &Player::Attack;

	ipActionPlayerStateMap[EInputAction::Right] = EPlayerState::IdleToRun;
	ipActionPlayerStateMap[EInputAction::Left] = EPlayerState::IdleToRun;
	ipActionPlayerStateMap[EInputAction::Jump] = EPlayerState::Jump;
	ipActionPlayerStateMap[EInputAction::Down] = EPlayerState::Crouch;
	ipActionPlayerStateMap[EInputAction::Attack] = EPlayerState::Attack;
	
	IdleAnimFunc = { playerAnim->GetIdleAnim(), &Player::Idle };
	IdleToRunAnimFunc = { playerAnim->GetIdleToRunAnim(), &Player::Run };
	RunToIdleAnimFunc = { playerAnim->GetRunToIdleAnim(), &Player::Run };
	RunAnimFunc = { playerAnim->GetRunAnim(), &Player::Run };
	FlipAnimFunc = { playerAnim->GetFlipAnim(), &Player:: Flip};
	JumpAnimFunc = { playerAnim->GetJumpAnim(), &Player::Jump };
	AttackAnimFunc = { playerAnim->GetAttackAnim(), &Player::Attack };

	playerStateFunctionMap[EPlayerState::IdleToRun] = IdleToRunAnimFunc;
	playerStateFunctionMap[EPlayerState::Run] = RunAnimFunc;
	playerStateFunctionMap[EPlayerState::Jump] = JumpAnimFunc;
	playerStateFunctionMap[EPlayerState::Attack] = AttackAnimFunc;
	playerStateFunctionMap[EPlayerState::Flip] = FlipAnimFunc;
}

void Player::InitRigidBody()
{
	if (ObjectRigidBody == nullptr) return;

	ObjectRigidBody->SetElasticity(0.f);
	ObjectRigidBody->SetGravityVisible(true);
	ObjectRigidBody->SetAccelerationAlpha({ 0.f, 800.f });
	ObjectRigidBody->SetMass(5.f);
	ObjectRigidBody->SetMaxVelocity({ 200.f, 400.f });
	ObjectRigidBody->SetFriction(300.f);
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
