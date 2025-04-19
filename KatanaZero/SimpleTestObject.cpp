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

	Speed = 300.f;

	return S_OK;
}

void SimpleTestObject::Update()
{
	LastPos = Pos;

	RigidBodyTest();

	Collision();

	//�����׷� �߰� (�ش翡�� ������ �޾Ƽ�  Render�� ȣ������ �������� ���� ����)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void SimpleTestObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// ��ũ���� �ʿ��� ������Ʈ��
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		scroll = Scroll;
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0);
	}
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