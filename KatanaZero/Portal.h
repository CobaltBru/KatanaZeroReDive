#pragma once
#include "GameObject.h"

enum class EPortal
{
	Start,
	Loop,
	Close,
	End
};

enum class EPortalType
{
	Attack,
	Object,
	End
};
class GPImage;
class Portal : public GameObject
{
public:
	Portal();
	~Portal() = default;
	HRESULT Init(FPOINT InPos,float InAngle, int InLoopCount, EPortalType InPortalType = EPortalType::Attack);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	void Attack();
	void Object();
private:
	GPImage* OpenImage;
	GPImage* CloseImage;
	GPImage* LoopImage;

	GPImage* CurrentImage;

	int Frame;
	float CurrentTime;
	float Angle;
	EPortal State;
	int LoopCount;
	int MaxLoopCount;
	EPortalType PortalType;
};

