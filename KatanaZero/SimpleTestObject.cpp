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
	:Image(nullptr)
{
}

// �׽�Ʈ �ڵ� 
HRESULT SimpleTestObject::Init(FPOINT InPos)
{
	Image = ImageManager::GetInstance()->FindImage("rocket");
	Pos = InPos;
	//�ݶ��̴� �߰�
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	ObjectRigidBody = new RigidBody(this);

	InitRegidBodySetting();

	ScrollSpeed = 300.f;

	return S_OK;
}

HRESULT SimpleTestObject::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	Pos = InPos;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);

	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);

	InitRegidBodySetting();

	ScrollSpeed = 300.f;

	return S_OK;
}

void SimpleTestObject::Update()
{
	LastPos = Pos;

	RigidBodyTest();

	Collision();

	//�����׷� �߰� (�ش翡�� ������ �޾Ƽ�  Render�� ȣ������ �������� ���� ����)
	RenderManager::GetInstance()->AddRenderGroup(RenderGroup, this);
}
 
void SimpleTestObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// ��ũ���� �ʿ��� ������Ʈ��
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0);
	}
}

void SimpleTestObject::InitRegidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// ź�� �������  0 ~ 1 ����
	ObjectRigidBody->SetElasticity(0.3f);

	// �߷� ����
	ObjectRigidBody->SetGravityVisible(true);
	// ���� 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//����
	ObjectRigidBody->SetMass(1.f);
	//�ִ� �ӵ�
	ObjectRigidBody->SetMaxVelocity({ 200.f,400.f });
	//����
	ObjectRigidBody->SetFriction(50.f);

	//������ �������� ���� ��
	//ObjectRigidBody->SetDown(true);
}

void SimpleTestObject::Collision()
{
	// �浹 ����
	FHitResult HitResult;

	// �� �ݶ��̴��� ECollisionGroup::Player�� �ִ� �ݶ��̴���� �浹ó��
	//if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
	//{
	//	// �浹�ߴ�.

	//	ObjectCollider->SetHit(true);	// �� �ݶ��̴� �浹
	//	HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

	//	HitResult.HitCollision->GetOwner();  // ���� ��ü ����
	//}
}

void SimpleTestObject::RigidBodyTest()
{
	if (ObjectRigidBody == nullptr)
		return;


	/*if (KeyManager::GetInstance()->IsStayKeyDown('O'))
	{
		ObjectRigidBody->AddForce({ 200.f,0.f });
	}*/

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