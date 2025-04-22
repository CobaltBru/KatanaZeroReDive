#include "SimpleObject.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "Image.h"
#include "KeyManager.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "ScrollManager.h"
#include "RigidBody.h"
#include "SimpleTestObject.h"
#include "CommonFunction.h"

#include "SnapShotManager.h"

SimpleObject::SimpleObject()
	:Image(nullptr), ScrollSpeed(0.f), bWall(false)
{
}

// �׽�Ʈ �ڵ� 
HRESULT SimpleObject::Init(FPOINT InPos, string InImageName)
{
	Image = ImageManager::GetInstance()->FindImage(InImageName);
	Pos = InPos;
	//�ݶ��̴� �߰�
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { (float)Image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(), 
		(float)Image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() }, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);

	InitRegidBodySetting();

	InitOffset();

	ScrollSpeed = 300.f;

	return S_OK;
}

HRESULT SimpleObject::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	Pos = InPos;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);

	InitRegidBodySetting();

	InitOffset();

	ScrollSpeed = 300.f;

	return S_OK;
}

void SimpleObject::Update()
{
	LastPos = Pos;

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
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0,false,true, ScrollManager::GetInstance()->GetScale());
	}
}

void SimpleObject::MakeSnapShot(void* out)
{
	PlayerSnapShot* pSnapShot = static_cast<PlayerSnapShot*>(out);
	pSnapShot->pos = this->Pos;
	pSnapShot->animFrame = 0;
	pSnapShot->bFlip = this->bFlip;
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

		HitResult.HitCollision->GetOwner();  // ���� ��ü ����adaad

		FPOINT pos;
		pos.x = HitResult.HitCollision->GetPos().x - ObjectCollider->GetPos().x;
		pos.y = HitResult.HitCollision->GetPos().y - ObjectCollider->GetPos().y;
		Normalize(pos);

		// ������ ������ٵ� ���� ����
		//HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(pos * 500.f);
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
	
	// ��ũ�� ������Ʈ (�÷��̾�)

	const float OffsetMinX = 200.f;
	const float OffsetMaxX = WINSIZE_X - 200.f;
	const float OffsetMinY = 100.f;
	const float OffsetMaxY = WINSIZE_Y - 100.f;

	const FPOINT Scroll = ScrollManager::GetInstance()->GetScrollOffset();

	FPOINT newScroll{};

	if (OffsetMaxX < Pos.x + Scroll.x)
		newScroll.x = -ScrollSpeed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMinX > Pos.x + Scroll.x && OffsetMinX < Pos.x)
		newScroll.x = ScrollSpeed * TimerManager::GetInstance()->GetDeltaTime();

	if (OffsetMaxY < Pos.y + Scroll.y)
		newScroll.y = -ScrollSpeed * TimerManager::GetInstance()->GetDeltaTime();
	if (OffsetMinY > Pos.y + Scroll.y && OffsetMinY < Pos.y)
		newScroll.y = ScrollSpeed * TimerManager::GetInstance()->GetDeltaTime();

	ScrollManager::GetInstance()->SetScroll(newScroll);
}

void SimpleObject::RigidBodyTest()
{
	if (ObjectRigidBody == nullptr)
		return;

	PhysicsMove();

	//NoPhysicsMove();

	ObjectRigidBody->Update();

	//������Ʈ �� �浹�� ������ ������ �޾� �� �� �ִ�.
	const FLineResult Result = ObjectRigidBody->GetResult();
	
	if (Result.LineType == ELineType::Wall)
	{
		ObjectRigidBody->SetAccelerationAlpha({ 0.f,500.f });
		bWall = true;

		const float L = Pos.x - (ObjectCollider->GetSize().x * 0.5f);
		const float R = Pos.x + (ObjectCollider->GetSize().x * 0.5f);

		const float LeftLength = abs(L - Result.OutPos.x);
		const float RightLength = abs(R - Result.OutPos.x);

		bIsLeft = Result.IsLeft;
	}
	else
	{
		ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
		bWall = false;
	}
}

void SimpleObject::InitRegidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// ź�� �������  0 ~ 1 ����
	ObjectRigidBody->SetElasticity(0.f);
	// �߷� ����
	ObjectRigidBody->SetGravityVisible(true);
	// ���� 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//����
	ObjectRigidBody->SetMass(5.f);
	//�ִ� �ӵ�
	ObjectRigidBody->SetMaxVelocity({ 200.f,400.f });
	//����
	ObjectRigidBody->SetFriction(300.f);
}

void SimpleObject::PhysicsMove()
{
	// �̲�������~ 

	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
		ObjectRigidBody->AddVelocity({ -200.f,0.f });
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
		ObjectRigidBody->AddVelocity({ 200.f,0.f });

	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
		ObjectRigidBody->SetDown(true);
	else
		ObjectRigidBody->SetDown(false);

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		if (bWall)
		{
			ObjectRigidBody->AddVelocity({ (bIsLeft ? 200.f : -200.f) ,-500.f });
		}
		else
		{
			ObjectRigidBody->AddVelocity({ 0.f,-300.f });
		}
	}
		
	//if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
	//	ObjectRigidBody->AddVelocity({ -200.f,0.f });
	//if (KeyManager::GetInstance()->IsOnceKeyDown('D'))
	//	ObjectRigidBody->AddVelocity({ 200.f,0.f });
}

void SimpleObject::NoPhysicsMove()
{
	ObjectRigidBody->SetVelocity({ 0.f,ObjectRigidBody->GetVelocity().y});

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
		ObjectRigidBody->AddVelocity({ 0.f,-200.f });
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
		ObjectRigidBody->AddVelocity({ -200.f,0.f });
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
		ObjectRigidBody->AddVelocity({ 200.f,0.f });

	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
		ObjectRigidBody->SetDown(true);
	else
		ObjectRigidBody->SetDown(false);
}

void SimpleObject::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}