#pragma once
#include "GameObject.h"

class GPImage;
class Syringe : public GameObject
{
public:
	Syringe();
	~Syringe() = default;
	HRESULT Init(FPOINT InPos, float InAngle);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	GPImage* gpImage;
	float Angle;
};

