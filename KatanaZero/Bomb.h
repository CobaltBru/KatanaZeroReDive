#pragma once
#include "GameObject.h"

enum class bombState
{
    Normal,
    Explosion
};

class RigidBody;
class Collider;
class Image;
class Bomb : public GameObject
{
private:
    Collider* BombCollider;
    RigidBody* ObjectRigidBody;

    
    int frameIndex;
    float angle;
    float timer;
    float timer2;
    bool isActive;

    bool isExploded = false;
    float lifeTimer = 0.f;  // 积己等 饶 矫埃 穿利

    Image* image;
    Image* explosionImage;

public:
    Bomb();
    virtual ~Bomb();

    virtual HRESULT Init(FPOINT pos, float angle);
    virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    bool GetIsActive() { return isActive; }
    bool GetIsExploded() { return isExploded; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->Pos = pos; }
    void SetAngle(float angle) { this->angle = angle; }

    
    void Collision();
};

