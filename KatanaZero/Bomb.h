#pragma once
#include "GameObject.h"

class RigidBody;
class Collider;
class Image;
class Bomb : public GameObject
{
private:
    Collider* BombCollider;

    float angle;
    bool isActive;

    Image* image;
    Image* explosionImage;

public:
    Bomb();
    virtual ~Bomb();

    virtual HRESULT Init(FPOINT pos, float angle);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    bool GetIsActive() { return isActive; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->Pos = pos; }
    void SetAngle(float angle) { this->angle = angle; }

    void Collision();
};

