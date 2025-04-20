#include "DefaultObject.h"
#include "Image.h"
#include "ImageManager.h"

#include "ScrollManager.h"

DefaultObject::DefaultObject()
    :Image(nullptr), bFlip(false)
{
}

HRESULT DefaultObject::Init(string InImageKey, FPOINT Pos, bool InFlip, ERenderGroup InRenderGroup)
{
    Pos = Pos;
    bFlip = InFlip;
    RenderGroup = InRenderGroup;

    Image = ImageManager::GetInstance()->FindImage(InImageKey);

    return S_OK;
}


void DefaultObject::Update()
{
    if (Image != nullptr)
        RenderManager::GetInstance()->AddRenderGroup(RenderGroup, this);
}

void DefaultObject::Render(HDC hdc)
{
    const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
    Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, bFlip);
}

void DefaultObject::Release()
{
}
