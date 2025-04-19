#pragma once
#include "GameObject.h"

class GPImage;
class Bullet1 : public GameObject
{
private:
    FPOINT startPos;

    float angle;
    bool isActive;

    GPImage* image;
    
public:
    Bullet1();
    virtual ~Bullet1();

    virtual HRESULT Init(FPOINT pos, float angle);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    void Fire(FPOINT startPos, FPOINT destPos);

    bool GetIsActive() { return isActive; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->startPos = pos; }
    void SetAngle(float angle) { this->angle = angle; }
};

