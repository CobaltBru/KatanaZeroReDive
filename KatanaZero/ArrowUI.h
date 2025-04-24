#pragma once
#include "GameObject.h"

class Image;
class ArrowUI : public GameObject
{
public:
	ArrowUI();
	~ArrowUI() = default;
	HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	bool GetVisible() const { return bVisible; }
	void SetVisible(bool InVisible) { bVisible = InVisible; }

private:
	Image* Image;
	float CurrentTime;
	int FrameX;
	bool bVisible;
	float Angle;
	float MoveY;
};

