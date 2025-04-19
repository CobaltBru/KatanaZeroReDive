#include "SimpleTestObject.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "Image.h"
#include "KeyManager.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "ScrollManager.h"
#include "RigidBody.h"
#include "SimpleObject.h"

SimpleTestObject::SimpleTestObject()
	:Image(nullptr), Speed(0.f), bJump(false), dY(-10.f), Gravity(0.1f), bFalling(true), bDown(false)
{
}

// 테스트 코드 
HRESULT SimpleTestObject::Init(FPOINT InPos)
{
	Image = ImageManager::GetInstance()->FindImage("rocket");
	Pos = InPos;
	//콜라이더 추가
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	ObjectRigidBody = new RigidBody(this);

	Speed = 300.f;

	return S_OK;
}

void SimpleTestObject::Update()
{
	LastPos = Pos;

	RigidBodyTest();

	Collision();

	//렌더그룹 추가 (해당에서 조건을 달아서  Render를 호출할지 안할지도 설정 가능)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void SimpleTestObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// 스크롤이 필요한 오브젝트들
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		scroll = Scroll;
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0);
	}
}

void SimpleTestObject::Collision()
{
	// 충돌 정보
	FHitResult HitResult;

	// 내 콜라이더와 ECollisionGroup::Player에 있는 콜라이더들과 충돌처리
	//if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
	//{
	//	// 충돌했다.

	//	ObjectCollider->SetHit(true);	// 내 콜라이더 충돌
	//	HitResult.HitCollision->SetHit(true);// 상대방 콜라이더 충돌

	//	HitResult.HitCollision->GetOwner();  // 상대방 객체 접근
	//}
}

void SimpleTestObject::RigidBodyTest()
{
	if (ObjectRigidBody == nullptr)
		return;


	if (KeyManager::GetInstance()->IsStayKeyDown('O'))
	{
		ObjectRigidBody->AddForce({ 200.f,0.f });
	}

	ObjectRigidBody->Update();
}

void SimpleTestObject::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}