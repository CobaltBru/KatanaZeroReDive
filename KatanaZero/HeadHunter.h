#pragma once
#include "GameObject.h"

enum class State
{
    Idle,
    GroundLazer,
    GroundGun,
    Bullet,
    Teleport,
    VerticalLazer,
    RoundLazer,
    Dash,
    DashDown,
    Faint,
    Die
};

class Player;
class RigidBody;
class Bomb;
class Lazer;
class Image;
class Bullet1;
class HeadHunter : public GameObject
{
private:
   
    FPOINT firePos;
    FPOINT wallPos;

    bool isFlip;
    bool isAttacked;
    bool isDead;
    bool isLeft;
    bool bCanSpawnBomb;
    bool faintStart;
    bool isFired;
    bool isFlipLocked;

    FPOINT jumpDist1;
    FPOINT jumpDist2;
    FPOINT jumpDist3;
    float angle;
    float weaponAngle; // 레이저 각도
    float dAngle; // 
    float timer;
    float moveTimer;
    float bulletTimer;


    int hp;
    int wave;
    int bulletWave;
    int dashWave;
    int lazerWave;
    int gunWave;
    int gunCount;
    int loop;
    int lazerLoop;
    int deadCount;
    int dieIndex;

    int frameIndex;
    Image* image;
    State state;
    Lazer* lazer;
    GameObject* player;
    std::vector<Bullet1*> bullets;
    std::vector<Bomb*> bombs;
    
    RigidBody* ObjectRigidBody;

    // test // 삭제 혹은 교체 예정
    FPOINT playerPos;

public:
    HeadHunter();
    virtual ~HeadHunter();

    virtual HRESULT Init(FPOINT InPos);
    virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    void SetState(State state) { this->state = state; }

    void Collision();

    void Idle();
    void GroundLazer();
    void GroundGun();
    void Bullet();
    void Teleport();
    void VerticalLazer();
    void RoundLazer();
    void Dash();
    void DashDown();
    void Faint();
    void Die();

    void SpawnBullet(FPOINT firePos, float angle);
    void SpawnBomb(FPOINT firePos, float angle);

    void ChangeState(State newState);
    void CheckPlayerPos();
    void RandomLoop();
    void IsLeft();
    void IsAttacked();
};

