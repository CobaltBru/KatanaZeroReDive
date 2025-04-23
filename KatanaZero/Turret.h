#pragma once
#include "GameObject.h"

class Bullet;
class Image;
class Turret : public GameObject
{
private:

    float timer;
    float frameIndex;

    bool isDead;

    Image* turretImage;
    Image* wallImage;
public:
    Turret();
    virtual ~Turret();

    virtual HRESULT Init(FPOINT pos);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);
};

