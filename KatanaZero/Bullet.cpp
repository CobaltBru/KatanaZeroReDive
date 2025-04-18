#include "Bullet.h"
#include "GPImage.h"

Bullet1::Bullet1()
{
}

Bullet1::~Bullet1()
{
}

HRESULT Bullet1::Init()
{
    image = new GPImage();
    image->AddImage(L"Image/bullet.png");

    return S_OK;
}

void Bullet1::Release()
{
    if (image)
    {
        delete image;
        image = nullptr;
    }
}

void Bullet1::Update(FPOINT pos, float angle)
{
    if(isActive)
    {
        this->startPos = pos;
        this->angle = angle;
    }
}

void Bullet1::Render(HDC hdc)
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

void Bullet1::Fire(FPOINT startPos, FPOINT destPos)
{
}
