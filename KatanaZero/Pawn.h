#pragma once
#include "GameObject.h"


class Image;
class PlayerController;
class Pawn: public GameObject
{
private:
	Image* image;
	RECT rc;

	PlayerController* Controller;

public:
	Pawn();
	virtual ~Pawn() {};

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

};

