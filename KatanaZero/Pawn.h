#pragma once
#include "GameObject.h"


class Image;
class PlayerController;
class Pawn: public GameObject
{
protected:
	Image* image;
	int FrameIndex;

	float frameTimer;

	RECT rc;
public:
	Pawn();
	virtual ~Pawn() {};

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

	inline int GetFrameIndex() { return FrameIndex; }
	inline void SetFrameIndex(int index) { FrameIndex = index; }
};

