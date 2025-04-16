#pragma once
#include "Pawn.h"


class Collider;
class Character: public Pawn
{
private:
	Collider* CharacterCollider;


public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

	void Move();
	void Jump();
	void Attack();
	void Down();

	void WallSlide();
};

