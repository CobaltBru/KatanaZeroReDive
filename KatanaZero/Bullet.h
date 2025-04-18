#pragma once
#include "GameObject.h"

class GPImage;
class Bullet1 : public GameObject
{
private:
    FPOINT startPos;
    FPOINT destPos;

    float angle;
    bool isActive;

    GPImage* image;
    
public:
    Bullet1();
    virtual ~Bullet1();

    virtual HRESULT Init();
    virtual void Release();
    virtual void Update(FPOINT pos, float angle);
    virtual void Render(HDC hdc);

    void Fire(FPOINT startPos, FPOINT destPos);

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->startPos = pos; }
};

