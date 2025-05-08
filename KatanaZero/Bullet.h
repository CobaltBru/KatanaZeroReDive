#pragma once
#include "GameObject.h"
#include "CollisionManager.h"
#include "Collider.h"

class GPImage;
class Bullet1 : public GameObject
{
private:
    float angle;
    bool isActive;
    float speed;
    int dir;

    GPImage* image;
    ECollisionGroup targetType;
public:
    Bullet1();
    virtual ~Bullet1();

    virtual HRESULT Init(FPOINT pos, float angle);
    virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);

    virtual HRESULT Init(FPOINT pos, float angle, float speed, int dir);

    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    bool GetIsActive() { return isActive; }
    float GetAngle() { return angle; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->Pos = pos; }
    void SetAngle(float angle) { this->angle = angle; }
    void SetTargetType(ECollisionGroup InCollision) { targetType = InCollision; }

    void Collision();
};

class BulletTest : public Bullet1
{
};

