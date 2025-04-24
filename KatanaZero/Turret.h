#pragma once
#include "GameObject.h"

enum class TurretState
{
    Spawn,
    Operate,
    Die
};

class GPImage;
class Bullet1;
class Image;
class Turret : public GameObject
{
private:

    float timer;
    float frameIndex;
    float angle;

    bool isDead;
    bool isActive;
    bool isDestroyed;
    bool isOperated;
    

    TurretState state;
    GPImage* headImage; // ÅÍ·¿ ´ë°¡¸®
    GPImage* backImage; // ÅÍ·¿ ÁöÁö´ë ¿·(µŞÂÊ)
    GPImage* frontImage; //ÅÍ·¿ ÁöÁö´ë ¿·(¾ÕÂÊ)
    Image* turretImage;

    GameObject* player;

    Image* wallImage;
    std::vector<Bullet1*> bullets;

    
public:
    Turret();
    virtual ~Turret();

    virtual HRESULT Init();
    virtual void Release();
    virtual void Update(FPOINT pos);
    virtual void Render(HDC hdc);

    void SetIsActive(bool isActive) { this->isActive = isActive; }

    void Collision();

    void SpawnBullet(FPOINT firePos, float angle);
};

