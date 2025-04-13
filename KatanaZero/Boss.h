#pragma once
#include "GameObject.h"


enum class State
{
	Idle,
	TakeOutRifle,
	Jump,
	JumpToCeil,
	Lazer,
	Sword,
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
	FPOINT wallPos;
	FPOINT ceilPos;
	int health;
	bool isFlip;
	bool isAttacked;
	bool isDead;
	//RECT hitbox;
	State state;
	Image* image;
	int framecount;
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
	void Jump();
	void JumpToCeil();
	void LazerAttack();
	void SwordAttack(GameObject* player);
	void BulletAttack();
	void FaintAttack();
	void Teleport();
	void Explode();
	void Die();
};

