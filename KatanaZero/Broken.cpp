#include "Broken.h"
#include "Image.h"
#include "ImageManager.h"
#include "Collider.h"
#include "RigidBody.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "CommonFunction.h"
#include "SoundManager.h"

uniform_int_distribution<int> uidFlip(0, 1);

Broken::Broken()
	:Count(0)
{
}

HRESULT Broken::Init(string InImageKey, FPOINT InPos, FPOINT InVelocity, float InMass)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey + "_broke");
	Pos = InPos;

	bFlip = uidFlip(dre);

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, Image->GetFrameWidth() * (Scale * ScrollManager::GetInstance()->GetScale()), false, 1.f);
	ObjectCollider->SetPos(Pos);


	ObjectRigidBody = new RigidBody(this);

	// ź�� �������  0 ~ 1 ����
	ObjectRigidBody->SetElasticity(0.5f);
	// �߷� ����
	ObjectRigidBody->SetGravityVisible(true);
	// ���� 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//����
	ObjectRigidBody->SetMass(InMass);
	//�ִ� �ӵ�
	ObjectRigidBody->SetMaxVelocity({ 300.f,600.f });
	//����
	ObjectRigidBody->SetFriction(200.f);

	ObjectRigidBody->SetVelocity(InVelocity);
	return S_OK;
}


void Broken::Update()
{
	LastPos = Pos;

	ObjectCollider->Update();
	ObjectRigidBody->Update();

	FPOINT Velocity = ObjectRigidBody->GetVelocity();
	FLineResult Result = ObjectRigidBody->GetResult();
	if (Result.LineType != ELineType::End && abs(Velocity.y) >= 1.f &&  GetLength(Velocity) >= 50.f && Count <= 2)
	{
		++Count;
		const bool type = uidFlip(dre);
		if (type)
		{
			SoundManager::GetInstance()->PlaySounds("sound_clatter_glass1", EChannelType::Broken1);
		}
		else
			SoundManager::GetInstance()->PlaySounds("sound_clatter_glass2", EChannelType::Broken2);
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Broken::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, bFlip, true, Scale * ScrollManager::GetInstance()->GetScale());

	if (ObjectCollider->CanDebugDraw())
		ObjectCollider->Render(hdc);
}

void Broken::Release()
{
	if (ObjectCollider != nullptr)
	{
		ObjectCollider->Release();
		delete ObjectCollider;
		ObjectCollider = nullptr;
	}

	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}