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
    virtual HRESULT Init(FPOINT pos, float angle, float speed, int dir);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    bool GetIsActive() { return isActive; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->Pos = pos; }
    void SetAngle(float angle) { this->angle = angle; }

    void Collision();
};

