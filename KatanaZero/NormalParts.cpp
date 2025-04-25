#include "NormalParts.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "ScrollManager.h"

NormalParts::NormalParts()
	:FrameX(0), CurrentTime(0.f), bLastFrameStop(false)
{
}

HRESULT NormalParts::Init(string InImageKey, FPOINT InPos,ERenderGroup InRenderGroup)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	CurrentImageKey = InImageKey;
	Pos = InPos;
	RenderGroup = InRenderGroup;
	return S_OK;
}

void NormalParts::Update()
{
	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 0.1f)
	{
		CurrentTime = 0.f;
	
		if (Image->GetMaxFrameX() - 1 == FrameX)
		{
			if(!bLastFrameStop)
				FrameX = 0;
		}			
		else
			++FrameX;
	}

	RenderManager::GetInstance()->AddRenderGroup(RenderGroup, this);
}

void NormalParts::Render(HDC hdc)
{
	Image->FrameRender(hdc, Pos.x + ScrollManager::GetInstance()->GetScroll().x, Pos.y + ScrollManager::GetInstance()->GetScroll().y, FrameX, 0, false, true, ScrollManager::GetInstance()->GetScale());
}

void NormalParts::Release()
{
}

int NormalParts::GetMaxFrameX()
{
	return Image->GetMaxFrameX();
}

void NormalParts::ChangeImage(string InImageKey, bool InLastFrameStop)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	CurrentImageKey = InImageKey;
	CurrentTime = 0.f;
	FrameX = 0;
	bLastFrameStop = InLastFrameStop;
}
