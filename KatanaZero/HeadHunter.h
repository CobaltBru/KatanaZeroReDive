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
    Run,
    Faint
};

class Image;
class HeadHunter : public GameObject
{
private:
    FPOINT pos;

    bool isFlip;
    bool isAttacked;
    bool isDead;

    float angle;
    float timer;
    float indexTimer;
    int hp;
    int wave;
    int bulletWave;
    int dashWave;
    int lazerWave;
    int gunWave;
    int gunCount;
    int loop;

    int frameIndex;
    Image* image;
    State state;


    // test // 삭제 혹은 교체 예정
    FPOINT playerPos;

public:
    HeadHunter();
    virtual ~HeadHunter();

    virtual HRESULT Init();
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    void SetState(State state) { this->state = state; }

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
    void Run();


    void ChangeState(State newState);
    void PlayAnimation(string key);
    void CheckPlayerPos();
    void NextWave();

};

