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

TaeKyungObject::TaeKyungObject()
	:Image(nullptr), ObjectCollider(nullptr), Speed(0.f), bJump(false), dy(-10.f),gravity(0.1f), bFalling(true)
{
}

// 테스트 코드 
HRESULT TaeKyungObject::Init()
{
	Image = ImageManager::GetInstance()->FindImage("rocket");

	//콜라이더 추가
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	Speed = 300.f;

	InitOffset();

	return S_OK;
} 

void TaeKyungObject::Update()
{
	Move();

	Collision();

	Offset();

	//테스트 이펙트 사운드 재생
	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
		SoundManager::GetInstance()->PlaySounds("EffectTest",EChannelType::Effect);
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

void TaeKyungObject::Move()
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
		Pos.x -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
		Pos.x += Speed * TimerManager::GetInstance()->GetDeltaTime();
	//if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
	//	Pos.y -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	//else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
	//	Pos.y += Speed * TimerManager::GetInstance()->GetDeltaTime();

	Jump();

	FLineResult Result;
	if (bFalling && LineManager::GetInstance()->CollisionLine(Pos, Result))
	{
		switch (Result.LineType)
		{
		case ELineType::Normal:
			Pos.y = Result.OutPos.y;
			break;
		case ELineType::Wall:
			break;
		case ELineType::DownLine:
			Pos.y = Result.OutPos.y;
			break;
		case ELineType::Ceiling:
			break;
		}
		
		
		bJump = false;
		dy = -10.f;
	}	
}

void TaeKyungObject::Jump()
{
	if (!bJump && KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
		bJump = true;
	
	if (bJump)
	{
		// 그냥 조잡한 점프 공식..
		// 리지드 바디 구현하는게 좋을 듯.. ㅠ
		dy += gravity;
		Pos.y += dy * Speed * TimerManager::GetInstance()->GetDeltaTime();
  		if (dy >= 0.f)
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
	// 스크롤 업데이트 (플레이어)

	const float OffsetMinX = 200.f;
	const float OffsetMaxX = WINSIZE_X - 200.f;
	const float OffsetMinY = 100.f;
	const float OffsetMaxY = WINSIZE_Y - 100.f;

	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

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


void TaeKyungObject::Release()
{
}