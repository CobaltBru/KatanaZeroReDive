#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "Image.h"
#include "KeyManager.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "ScrollManager.h"

TaeKyungObject::TaeKyungObject()
	:Image(nullptr), ObjectCollider(nullptr), Speed(0.f)
{
}

// 테스트 코드 
HRESULT TaeKyungObject::Init()
{
	Image = ImageManager::GetInstance()->FindImage("rocket");
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	Speed = 300.f;

	return S_OK;
}

void TaeKyungObject::Update()
{
	Move();


	Offset();

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
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
		Pos.y -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
		Pos.y += Speed * TimerManager::GetInstance()->GetDeltaTime();
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