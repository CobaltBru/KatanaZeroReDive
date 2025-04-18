#include "Lazer.h"
#include "GPImage.h"

Lazer::Lazer()
{
}

Lazer::~Lazer()
{
}

HRESULT Lazer::Init()
{
    startPos = { 0,0 };
    destPos = { 0,0 };

    angle = 0;
    isActive = false;
    image = new GPImage();
    image->AddImage(L"Image/lazel_2.png", 4, 1);

    return S_OK;
}

void Lazer::Release()
{
    if (image)
    {
        delete image;
        image = nullptr;
    }
}

void Lazer::Update(FPOINT pos, float angle)
{
    if (isActive)
    {
        this->angle = angle;
        startPos = pos;
    }
}

void Lazer::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    if (image)
    {
        if (isActive)
        {
            image->Middle_RenderFrameAngle(&graphics, startPos, 0, angle, false, 1.0f);
        }
        
    }
    
}

void Lazer::Fire(FPOINT startPos, FPOINT destPos)
{

}
