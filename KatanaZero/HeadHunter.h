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
    Machine,
    Faint,
    Die
};

class RigidBody;
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

    int frameIndex;
    Image* image;
    State state;
    Lazer* lazer;
    std::vector<Bullet1*> bullets;
    RigidBody* ObjectRigidBody;

    // test // 삭제 혹은 교체 예정
    FPOINT playerPos;

public:
    HeadHunter();
    virtual ~HeadHunter();

    virtual HRESULT Init(FPOINT InPos);
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
    void Machine();
    void Die();

    void SpawnBullet(FPOINT firePos, float angle);

    void ChangeState(State newState);
    void CheckPlayerPos();
    void RandomLoop();
    void IsLeft();
  
};

