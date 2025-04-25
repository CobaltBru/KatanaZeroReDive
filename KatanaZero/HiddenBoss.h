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
	PortalAttack1,
	PortalAttack2,
	Weak,
	End
};

class NormalParts;
class ArmAttack;
class BossWeak;
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
	void PortalAttack1();
	void PortalAttack2();

	void Weak();
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

	int PortalCount;
	int MaxPortalCount;

	FPOINT NextPos;

	int Index;
	BossWeak* bossWeak;

	int Hp;
	float CurrentHitTime;
	float FaceHitTime;
};

