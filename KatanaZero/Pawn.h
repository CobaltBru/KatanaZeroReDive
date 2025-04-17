#pragma once
#include "GameObject.h"


class Image;
class PlayerController;
class Pawn: public GameObject
{
protected:
	Image* image;
	int FrameIndex;

	RECT rc;

public:
	Pawn();
	virtual ~Pawn() {};

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

};

