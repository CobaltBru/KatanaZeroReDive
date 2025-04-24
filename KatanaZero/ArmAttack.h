#pragma once
#include "GameObject.h"

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

private:
	Image* Image;
	GameObject* Target;
};

