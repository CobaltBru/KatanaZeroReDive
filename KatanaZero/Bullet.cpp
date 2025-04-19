#include "Bullet.h"
#include "GPImage.h"

Bullet1::Bullet1()
{
}

Bullet1::~Bullet1()
{
}

HRESULT Bullet1::Init(FPOINT pos, float angle)
{
    startPos = pos;
    this->angle = angle;

    isActive = true;

    image = new GPImage();
    image->AddImage(L"Image/HeadHunter/bullet.png");

    return S_OK;
}

void Bullet1::Release()
{
    if (image)
    {
        image->Release();
        delete image;
        image = nullptr;
    }
}

void Bullet1::Update()
{
    if(isActive)
    {
        startPos.x += cosf(DEG_TO_RAD(angle));
        startPos.y += sinf(DEG_TO_RAD(angle));

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
