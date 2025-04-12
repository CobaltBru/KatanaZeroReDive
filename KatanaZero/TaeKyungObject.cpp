#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "Image.h"
#include "KeyManager.h"

TaeKyungObject::TaeKyungObject()
	:Image(nullptr), Pos({}), Speed(0.f)
{
}

// �׽�Ʈ �ڵ� 
HRESULT TaeKyungObject::Init()
{
	Image = ImageManager::GetInstance()->FindImage("rocket");

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