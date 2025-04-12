#include "TestClass.h"
using namespace Gdiplus;

void TestClass::Init()
{
	pens[0] = new Pen(Color(255 , 255 , 0 , 0) , 2.0f);
	pens[1] = new Pen(Color(255 , 0 , 255 , 0) , 2.0f);
	pens[2] = new Pen(Color(255 , 0 , 0 , 255) , 2.0f);
	pens[3] = new Pen(Color(255 , 255 , 255 , 0) , 2.0f);
	pens[4] = new Pen(Color(255 , 0 , 255 , 255) , 2.0f);
}



void TestClass::RenderRect(HDC hdc , FPOINT pos , float width , float height, Pcolor color)
{
	Graphics graphics(hdc);
	graphics.DrawRectangle(pens[color] , RectF(pos.x , pos.y , width , height));
}

void TestClass::Middle_RenderRect(HDC hdc , FPOINT pos , float width , float height , Pcolor color)
{
	Graphics graphics(hdc);
	graphics.DrawRectangle(pens[color] , RectF(pos.x - width/2 , pos.y - height/2 , width , height));
}

void TestClass::Release()
{
	for ( int i = 0; i < 5; i++ )
	{
		delete pens[i];
	}
}
