#pragma once
#include "config.h"
class TestClass
{
private:
	Gdiplus::Pen* pens[5];
public:
	enum Pcolor { BLACK , WHITE , RED , GREEN , BLUE };

	void Init();

	void RenderRect(HDC hdc , FPOINT pos , float width , float height , Pcolor color);
	void Middle_RenderRect(HDC hdc , FPOINT pos , float width , float height , Pcolor color);

	void Release();
};

