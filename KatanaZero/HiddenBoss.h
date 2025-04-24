#pragma once
#include "GameObject.h"


class Image;
class GameObject;
struct FPartInfo
{
	Image* image;
	float FrameX;
};

enum class EState
{
	Idle,
	ArmAttack,
	End
};

class NormalParts;
class ArmAttack;
class HiddenBoss : public GameObject
{
public:
	HiddenBoss();
	~HiddenBoss() = default;

	virtual HRESULT Init();	
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

private:
	void Idle();
	void ArmAttackState();


private:
	float CurrentTime;
	NormalParts* Face;
	NormalParts* Arm;

	// Normal
	NormalParts* Body;
	NormalParts* ChillGuy;

	EState State;
	EState PreState;

	ArmAttack* armAttack;
};

