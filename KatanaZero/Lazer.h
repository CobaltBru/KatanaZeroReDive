#pragma once
#include "GameObject.h"


class GPImage;
class Lazer : public GameObject
{
private:

    int frameIndex;
    float angle;
    float timer;
    float timer2;
    float warningTime;
    bool isActive;
    bool bCanCollide;

    GPImage* image;

public:
    Lazer();
    virtual ~Lazer();

    virtual HRESULT Init();
    virtual void Release();
    virtual void Update(FPOINT pos, float angle);
    virtual void Render(HDC hdc);

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetbCanCollide(bool bCanCollide) { this->bCanCollide = bCanCollide; }
    void SetWarningTime(float time) { warningTime = time; }
    void Collision();
};

