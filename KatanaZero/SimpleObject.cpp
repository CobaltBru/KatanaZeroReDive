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

// �׽�Ʈ �ڵ� 
HRESULT SimpleObject::Init(FPOINT InPos)
{
	Image = ImageManager::GetInstance()->FindImage("rocket");
	Pos = InPos;
	//�ݶ��̴� �߰�
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

	// ��ġ�� ���� ��� ������ ���� �� �������� ȣ�� ����
	Offset();

	//�����׷� �߰� (�ش翡�� ������ �޾Ƽ�  Render�� ȣ������ �������� ���� ����)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void SimpleObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// ��ũ���� �ʿ��� ������Ʈ��
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
	// �浹 ����
	FHitResult HitResult;

	// �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy))
	{
		// �浹�ߴ�.

		ObjectCollider->SetHit(true);	// �� �ݶ��̴� �浹
		HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

		HitResult.HitCollision->GetOwner();  // ���� ��ü ����
	}
}

void SimpleObject::InitOffset()
{
	//��Ŀ���ؾ� �������� �� �ֵ�.
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
	// ������� ��ũ�� ������Ʈ ����
	if (SnapShotManager::GetInstance()->IsReplaying())
	{
		return;
	}
	// ��ũ�� ������Ʈ (�÷��̾�)

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