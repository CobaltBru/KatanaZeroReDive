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

// �׽�Ʈ �ڵ� 
HRESULT TaeKyungObject::Init()
{
	Image = ImageManager::GetInstance()->FindImage("rocket");

	//�ݶ��̴� �߰�
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

	//�׽�Ʈ ����Ʈ ���� ���
	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
		SoundManager::GetInstance()->PlaySounds("EffectTest",EChannelType::Effect);
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
		// �׳� ������ ���� ����..
		// ������ �ٵ� �����ϴ°� ���� ��.. ��
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
	// ��ũ�� ������Ʈ (�÷��̾�)

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