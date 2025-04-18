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

// �׽�Ʈ �ڵ� 
HRESULT TaeKyungObject::Init(FPOINT InPos)
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

void TaeKyungObject::Update()
{
	LastPos = Pos;

	RigidBodyTest();
	//Move();

	Collision();

	// ��ġ�� ���� ��� ������ ���� �� �������� ȣ�� ����
	Offset();

	//�׽�Ʈ ����Ʈ ���� ���
	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
		SoundManager::GetInstance()->PlaySounds("EffectTest", EChannelType::Effect);
	// ��� ���� ����
	if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
		SoundManager::GetInstance()->StopAll();

	//�����׷� �߰� (�ش翡�� ������ �޾Ƽ�  Render�� ȣ������ �������� ���� ����)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void TaeKyungObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// ��ũ���� �ʿ��� ������Ʈ��
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
	//���� �ִϸ��̼� ����� �����ӵ� ����
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

	// �����浹 �ѹ��� ó���� �� �ְ� ����� ���� �� ������
	//=========================================================================================================================
	// ���� ��
	//FLineResult Result;
	//if (LineManager::GetInstance()->CollisionWallLine(Pos, Result, ObjectCollider->GetSize()))
	//	Pos.x = Result.OutPos.x;

	//// ��
	//if (bFalling && LineManager::GetInstance()->CollisionLine(Pos, Result, ObjectCollider->GetSize().y, bDown))
	//{
	//	Pos.y = Result.OutPos.y;

	//	bJump = false;
	//	bDown = false;
	//	dY = -10.f;
	//}
	////  õ��
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
		// �׳� ������ ���� ����..
		// ������ �ٵ� �����ϴ°� ���� ��.. ��
		
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

void TaeKyungObject::InitOffset()
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

void TaeKyungObject::Offset()
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