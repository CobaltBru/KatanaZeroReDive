#include "TestObject.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"

TestObject::TestObject()
	:Image(nullptr)
{
}

void TestObject::Init(string InImageKey, FPOINT InPos)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	Pos = InPos;	
}

void TestObject::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void TestObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// 스크롤이 필요한 오브젝트들
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y,0,0);
	}
}

void TestObject::Release()
{
}