#pragma once
#include "GameObject.h"
#include "config.h"


class Image;
class GangsterBullet : public GameObject
{
private:
	Image* image;
    int dir;
    float speed;
    float force;
public:
    GangsterBullet();
    virtual ~GangsterBullet();
    virtual HRESULT Init(FPOINT pos, int dir);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);
    virtual void MakeSnapShot(void* out) override;

    void InitRigidBodySetting();
    void Collision();
    void Fire();
};

