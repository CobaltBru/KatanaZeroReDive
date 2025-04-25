#pragma once
#include "GameObject.h"

enum class bombState
{
    Normal,
    Explosion
};


class Image;
class Bomb : public GameObject
{
private:
  
    int frameIndex;

    float timer;
    float timer2;
    float speed; 
    float speed2;
    bool isActive;

    bool isExploded = false;
    float lifeTimer = 0.f;  // ������ �� �ð� ����

    Image* image;
    Image* explosionImage;

public:
    Bomb();
    virtual ~Bomb();

    virtual HRESULT Init(FPOINT pos, float angle, float speed);
    virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    bool GetIsActive() { return isActive; }
    bool GetIsExploded() { return isExploded; }

    void SetIsActive(bool isActive) { this->isActive = isActive; }
    void SetPos(FPOINT pos) { this->Pos = pos; }

    
    void Collision();
};

