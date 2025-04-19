#include "SimpleObject.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "Image.h"
#include "KeyManager.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "ScrollManager.h"
#include "SoundManager.h"
#include "LineManager.h"
#include "EffectManager.h"
#include "RigidBody.h"

#include "GPImage.h"


SimpleObject::SimpleObject()
	:Image(nullptr), ObjectCollider(nullptr), ObjectRigidBody(nullptr), Speed(0.f), bJump(false), dY(-10.f), Gravity(0.1f), bFalling(true), bDown(false)
{
}

// 테스트 코드 
HRESULT SimpleObject::Init(FPOINT InPos)
{
	Image = ImageManager::GetInstance()->FindImage("rocket");
	Pos = InPos;
	//콜라이더 추가
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectRigidBody = new RigidBody(this);

	Speed = 300.f;

	InitOffset();

	return S_OK;
}

void SimpleObject::Update()
{
	LastPos = Pos;

	//Move();
	RigidBodyTest();


	Collision();

	// 위치에 관한 모든 로직이 끝난 뒤 마지막에 호출 권장
	Offset();

	//렌더그룹 추가 (해당에서 조건을 달아서  Render를 호출할지 안할지도 설정 가능)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void SimpleObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// 스크롤이 필요한 오브젝트들
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		scroll = Scroll;
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0);
	}
}

void SimpleObject::Move()
{
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		Pos.x -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	}

	else if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		Pos.x += Speed * TimerManager::GetInstance()->GetDeltaTime();
	}

	if (!bJump && KeyManager::GetInstance()->IsOnceKeyDown('W') || KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		bJump = true;
		dY = -10.f;
	}
	if (!bJump && KeyManager::GetInstance()->IsOnceKeyDown('S'))
	{
		if (!bDown)
			dY = 0.f;
		bDown = true;
	}

	Jump();
}

void SimpleObject::Jump()
{
	if (bJump || bDown)
	{
		dY += Gravity;
		Pos.y += dY * Speed * TimerManager::GetInstance()->GetDeltaTime();
		if (dY >= 0.f)
			bFalling = true;
		else
			bFalling = false;
	}
}

void SimpleObject::Collision()
{
	// 충돌 정보
	FHitResult HitResult;

	// 내 콜라이더와 ECollisionGroup::Enemy에 있는 콜라이더들과 충돌처리
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy))
	{
		// 충돌했다.

		ObjectCollider->SetHit(true);	// 내 콜라이더 충돌
		HitResult.HitCollision->SetHit(true);// 상대방 콜라이더 충돌

		HitResult.HitCollision->GetOwner();  // 상대방 객체 접근
	}
}

void SimpleObject::InitOffset()
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

void SimpleObject::Offset()
{
	if (!ScrollManager::GetInstance()->IsFocus())
		return;
	// 역재생시 스크롤 업데이트 따로
	if (SnapShotManager::GetInstance()->IsReplaying())
	{
		return;
	}
	// 스크롤 업데이트 (플레이어)

	const float OffsetMinX = 200.f;
	const float OffsetMaxX = WINSIZE_X - 200.f;
	const float OffsetMinY = 100.f;
	const float OffsetMaxY = WINSIZE_Y - 100.f;

	const FPOINT Scroll = ScrollManager::GetInstance()->GetScrollOffset();

	FPOINT newScroll{};

	if (OffsetMaxX < Pos.x + Scroll.x)
		newScroll.x = -Speed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMinX > Pos.x + Scroll.x && OffsetMinX < Pos.x)
		newScroll.x = Speed * TimerManager::GetInstance()->GetDeltaTime();

	if (OffsetMaxY < Pos.y + Scroll.y)
		newScroll.y = -Speed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMinY > Pos.y + Scroll.y && OffsetMinY < Pos.y)
		newScroll.y = Speed * TimerManager::GetInstance()->GetDeltaTime();

	ScrollManager::GetInstance()->SetScroll(newScroll);
}

void SimpleObject::RigidBodyTest()
{
	if (ObjectRigidBody == nullptr)
		return;

	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
		ObjectRigidBody->AddForce({ -200.f,0.f });
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
		ObjectRigidBody->AddForce({ 200.f,0.f });

	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
		ObjectRigidBody->SetDown(true);
	else
		ObjectRigidBody->SetDown(false);

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
		ObjectRigidBody->AddVelocity({ 0.f,-200.f });

	if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
		ObjectRigidBody->AddVelocity({ -200.f,0.f });
	if (KeyManager::GetInstance()->IsOnceKeyDown('D'))
		ObjectRigidBody->AddVelocity({ 200.f,0.f });

	ObjectRigidBody->Update();
}

void SimpleObject::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}