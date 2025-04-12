#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "Image.h"
#include "KeyManager.h"
#include "CollisionManager.h"
#include "Collider.h"

TaeKyungObject::TaeKyungObject()
	:Image(nullptr), ObjectCollider(nullptr), Speed(0.f)
{
}

// �׽�Ʈ �ڵ� 
HRESULT TaeKyungObject::Init()
{
	Image = ImageManager::GetInstance()->FindImage("rocket");
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	Speed = 100.f;

	return S_OK;
}

void TaeKyungObject::Update()
{
	Move();


	//�����׷� �߰� (�ش翡�� ������ �޾Ƽ�  Render�� ȣ������ �������� ���� ����)
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void TaeKyungObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		Image->FrameRender(hdc, Pos.x, Pos.y, 0, 0);
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


void TaeKyungObject::Release()
{
}