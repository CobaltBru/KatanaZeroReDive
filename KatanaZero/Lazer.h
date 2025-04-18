#pragma once
#include "GameObject.h"

class GPImage;
class Lazer : public GameObject
{
private:
    FPOINT startPos;
    FPOINT destPos;

    float angle;
    bool isActive;

    GPImage* image;

public:
    Lazer();
    virtual ~Lazer();

    virtual HRESULT Init();
    virtual void Release();
    virtual void Update(FPOINT pos, float angle);
    virtual void Render(HDC hdc);

    void Fire(FPOINT startPos, FPOINT destPos);

    void SetIsActive(bool isActive) { this->isActive = isActive; }

};

