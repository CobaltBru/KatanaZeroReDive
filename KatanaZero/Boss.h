#pragma once
#include "GameObject.h"


enum class State
{
	Idle,
	TakeOutRifle,
	Jump,
	JumpIdle,
	JumpToCeil,
	Lazer,
	ReadyToDash,
	Dash,
	DashEnd,
	Bullet,
	Faint,
	Teleport,
	Explode,
	Die
};


class Image;
class Boss : public GameObject
{
private:
	
	FPOINT pos;
	FPOINT ceilPos;
	FPOINT wallPos;
	FPOINT landPos;

	// 플레이어 바라보기 테스트용
	FPOINT playerPos;

	int health;
	bool isFlip;
	bool isAttacked;
	bool isDead;
	bool isLeft;
	bool isRetreating;
	float angle;
	
	//RECT hitbox;
	State state;
	Image* image;
	int frameIndex;
	float timer;
	int LoopCount;
public:
	Boss();
	virtual ~Boss();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void Idle();
	void TakeOutRifleAttack();
	void TakeOutWeapon(string key);
	void Jump();
	void JumpToCeil();
	void LazerAttack();
	void ReadyToDash();
	void Dash();
	void DashEnd();
	void BulletAttack();
	void Faint();
	void Teleport();
	void Explode();
	void Die();
	void CheckLeft();
	void CheckPlayerPos(); // 나중에 player위치 받아오기
};

