#pragma once
#include "GameObject.h"

class Collider;
class GPImage;
class Bullet1 : public GameObject
{
private:
    Collider* BulletCollider;

    float angle;
    bool isActive;

    GPImage* image;
    
public:
    Bullet1();
    virtual ~Bullet1();

    virtual HRESULT Init(FPOINT pos, float angle);
    virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    bool GetIsActive() { return isActive; }
    float GetAngle() { return angle; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->Pos = pos; }
    void SetAngle(float angle) { this->angle = angle; }

    void Collision();
};

class BulletTest : public Bullet1
{
};

