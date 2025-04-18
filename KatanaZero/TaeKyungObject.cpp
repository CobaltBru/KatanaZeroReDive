#include "TaeKyungObject.h"
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


TaeKyungObject::TaeKyungObject()
	:Image(nullptr), ObjectCollider(nullptr), ObjectRigidBody(nullptr), Speed(0.f), bJump(false), dY(-10.f), Gravity(0.1f), bFalling(true), bDown(false)
{
}

// 테스트 코드 
HRESULT TaeKyungObject::Init(FPOINT InPos)
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

void TaeKyungObject::Update()
{
	LastPos = Pos;

	RigidBodyTest();
	//Move();

	Collision();

	// 위치에 관한 모든 로직이 끝난 뒤 마지막에 호출 권장
	Offset();

	//테스트 이펙트 사운드 재생
	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
		SoundManager::GetInstance()->PlaySounds("EffectTest", EChannelType::Effect);
	// 모든 음악 끄기
	if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
		SoundManager::GetInstance()->StopAll();

	//렌더그룹 추가 (해당에서 조건을 달아서  Render를 호출할지 안할지도 설정 가능)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void TaeKyungObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// 스크롤이 필요한 오브젝트들
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0);
	}
}

void TaeKyungObject::MakeSnapShot(void* out)
{
	PlayerSnapShot* pSnapShot = static_cast<PlayerSnapShot*>(out);
	pSnapShot->pos = this->GetPos();
	pSnapShot->animFrame = 0;
}

void TaeKyungObject::ApplySnapShot(const PlayerSnapShot& snapShot)
{
	this->Pos = snapShot.pos;
	//추후 애니메이션 생기면 프레임도 수정
	//this->animFrame = snapShot.animFrame;
}


void TaeKyungObject::Move()
{
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
		Pos.x -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown('D'))
		Pos.x += Speed * TimerManager::GetInstance()->GetDeltaTime();
	if (!bJump && KeyManager::GetInstance()->IsOnceKeyDown('W') || KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		bJump = true;
		EffectManager::GetInstance()->Activefx("jumpcloud", { this->Pos.x, this->Pos.y }, 0.0f, false);
		dY = -10.f;
	}
	if (!bJump && KeyManager::GetInstance()->IsOnceKeyDown('S'))
	{
		if (!bDown)
			dY = 0.f;
		bDown = true;
	}

	Jump();

	// 라인충돌 한번에 처리할 수 있게 만들면 좋을 것 같은데
	//=========================================================================================================================
	// 수직 벽
	//FLineResult Result;
	//if (LineManager::GetInstance()->CollisionWallLine(Pos, Result, ObjectCollider->GetSize()))
	//	Pos.x = Result.OutPos.x;

	//// 땅
	//if (bFalling && LineManager::GetInstance()->CollisionLine(Pos, Result, ObjectCollider->GetSize().y, bDown))
	//{
	//	Pos.y = Result.OutPos.y;

	//	bJump = false;
	//	bDown = false;
	//	dY = -10.f;
	//}
	////  천장
	//else if (!bFalling && LineManager::GetInstance()->CollisionCeilingLine(Pos, Result, ObjectCollider->GetSize().y))
	//{
	//	Pos.y = Result.OutPos.y;
	//	dY = 0.f;
	//}
	//=========================================================================================================================
}

void TaeKyungObject::Jump()
{
	if (bJump || bDown)
	{
		// 그냥 조잡한 점프 공식..
		// 리지드 바디 구현하는게 좋을 듯.. ㅠ
		
		dY += Gravity;
		Pos.y += dY * Speed * TimerManager::GetInstance()->GetDeltaTime();
		if (dY >= 0.f)
			bFalling = true;
		else
			bFalling = false;
	}
}

void TaeKyungObject::Collision()
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

void TaeKyungObject::InitOffset()
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

void TaeKyungObject::Offset()
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

void TaeKyungObject::RigidBodyTest()
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

void TaeKyungObject::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}