#pragma once
#include "GameObject.h"

class Image;
class Broken : public GameObject
{
public:
	Broken();
	~Broken() = default;

	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InVelocity, float InMass);

	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	Image* Image;
	int Count;
};

