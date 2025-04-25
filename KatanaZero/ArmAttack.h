#pragma once
#include "GameObject.h"

enum class EArmState
{
	Start,
	Attack,
	Follow,
	High,
	Cooldown,
	End
};

class Image;
class ArmAttack : public GameObject
{
public:
	ArmAttack();
	~ArmAttack() = default;

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	bool IsFinish() const { return bFinish; }

private:
	float CurrentTime;
	Image* Image;
	GameObject* Target;

	EArmState State;
	float FollowTime;
	int AttackCount;
	bool bFinish;
};

