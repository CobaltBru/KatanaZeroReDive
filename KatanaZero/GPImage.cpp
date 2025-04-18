#include "GPImage.h"
using namespace Gdiplus;

#ifdef new
#undef new
#endif

Gdiplus::Pen* GPImage::pens[5];

void GPImage::Init()
{
	pens[0] = new Pen(Color(255 , 255 , 0 , 0) , 2.0f);
	pens[1] = new Pen(Color(255 , 0 , 255 , 0) , 2.0f);
	pens[2] = new Pen(Color(255 , 0 , 0 , 255) , 2.0f);
	pens[3] = new Pen(Color(255 , 255 , 255 , 0) , 2.0f);
	pens[4] = new Pen(Color(255 , 0 , 255 , 255) , 2.0f);
}

void GPImage::AddImage(const wchar_t* filePath)
{
	pBitmap = (Bitmap*)Gdiplus::Image::FromFile(filePath);
	width = pBitmap->GetWidth();
	height = pBitmap->GetHeight();
	maxFrameX = 1;
	maxFrameY = 1;
	frameWidth = width;
	frameHeight = height;
}

void GPImage::AddImage(const wchar_t* filePath, int frameX, int frameY)
{
	pBitmap = (Bitmap*)Gdiplus::Image::FromFile(filePath);
	width = pBitmap->GetWidth();
	height = pBitmap->GetHeight();
	maxFrameX = frameX;
	maxFrameY = frameY;
	frameWidth = width / frameX;
	frameHeight = height / frameY;
}



void GPImage::RenderRect(Gdiplus::Graphics* graphics , FPOINT pos , float width , float height, Pcolor color)
{
	graphics->DrawRectangle(pens[color] , RectF(pos.x , pos.y , width , height));
}

void GPImage::Middle_RenderRect(Gdiplus::Graphics* graphics , FPOINT pos , float width , float height , Pcolor color)
{
	graphics->DrawRectangle(pens[color] , RectF(pos.x - width/2 , pos.y - height/2 , width , height));
}

void GPImage::Render(Gdiplus::Graphics* graphics, FPOINT pos)
{
	graphics->DrawImage(pBitmap, pos.x, pos.y);
}

void GPImage::Middle_Render(Gdiplus::Graphics* graphics, FPOINT pos)
{
	graphics->DrawImage(pBitmap, pos.x - width / 2, pos.y - height / 2);
}

void GPImage::Render(Gdiplus::Graphics* graphics, FPOINT pos, bool flip, float alpha)
{
	
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	if (!flip)
	{
		Rect destRect(pos.x, pos.y, width, height);
		graphics->DrawImage(pBitmap, destRect, 0, 0,
			width, height, UnitPixel, &imgAttr);
	}
	else
	{
		Rect destRect(pos.x + width, pos.y, -width, height);
		graphics->DrawImage(pBitmap, destRect, 0, 0,
			width, height, UnitPixel, &imgAttr);
	}
	
}

void GPImage::Middle_Render(Gdiplus::Graphics* graphics, FPOINT pos, bool flip, float alpha)
{
	ColorMatrix colorMatrix = {
		1.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	
	if (!flip)
	{
		Rect destRect(pos.x - width / 2, pos.y - height / 2, width, height);
		graphics->DrawImage(pBitmap, destRect, 0, 0,
			width, height, UnitPixel, &imgAttr);
	}
	else
	{
		Rect destRect(pos.x + width / 2, pos.y - height / 2, -width, height);
		graphics->DrawImage(pBitmap, destRect, 0, 0,
			width, height, UnitPixel, &imgAttr);
	}
}

void GPImage::RenderFrame(Gdiplus::Graphics* graphics, FPOINT pos, int frame, bool flip, float alpha)
{
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	

	int col = frame % maxFrameX;
	int row = frame / maxFrameX;
	float srcX = (float)(col * frameWidth);
	float srcY = (float)(row * frameHeight);

	if (!flip)
	{
		Rect destRect(pos.x, pos.y, frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	else
	{
		Rect destRect(pos.x+ frameWidth, pos.y, -frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	
}

void GPImage::Middle_RenderFrame(Gdiplus::Graphics* graphics, FPOINT pos, int frame, bool flip, float alpha)
{
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	

	int col = frame % maxFrameX;
	int row = frame / maxFrameX;
	float srcX = (float)(col * frameWidth);
	float srcY = (float)(row * frameHeight);
	if (!flip)
	{
		Rect destRect(pos.x - frameWidth / 2, pos.y - frameHeight / 2, frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	else
	{
		Rect destRect(pos.x + frameWidth / 2, pos.y - frameHeight / 2, -frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	
}

void GPImage::RenderFrameAngle(Gdiplus::Graphics* graphics, FPOINT pos, int frame, float angle, bool flip, float alpha)
{
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	
	int col = frame % maxFrameX;
	int row = frame / maxFrameX;
	float srcX = (float)(col * frameWidth);
	float srcY = (float)(row * frameHeight);
	Gdiplus::Matrix old;
	graphics->GetTransform(&old);
	Gdiplus::Matrix matrix;
	matrix.RotateAt(angle, Gdiplus::PointF(pos.x + frameWidth / 2, pos.y + frameHeight / 2));
	graphics->SetTransform(&matrix);
	if (!flip)
	{
		Rect destRect(pos.x, pos.y, frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	else
	{
		Rect destRect(pos.x + frameWidth, pos.y, -frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	graphics->SetTransform(&old);
}

void GPImage::Middle_RenderFrameAngle(Gdiplus::Graphics* graphics, FPOINT pos, int frame, float angle, bool flip, float alpha)
{
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);


	int col = frame % maxFrameX;
	int row = frame / maxFrameX;
	float srcX = (float)(col * frameWidth);
	float srcY = (float)(row * frameHeight);
	Gdiplus::Matrix old;
	graphics->GetTransform(&old);
	Gdiplus::Matrix matrix;
	matrix.RotateAt(angle, Gdiplus::PointF(pos.x, pos.y));
	graphics->SetTransform(&matrix);
	if (!flip)
	{
		Rect destRect(pos.x - frameWidth / 2, pos.y - frameHeight / 2, frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	else
	{
		Rect destRect(pos.x + frameWidth / 2, pos.y - frameHeight / 2, -frameWidth, frameHeight);
		graphics->DrawImage(pBitmap, destRect, srcX, srcY,
			frameWidth, frameHeight, UnitPixel, &imgAttr);
	}
	graphics->SetTransform(&old);
}

void GPImage::RenderAll(Gdiplus::Graphics* graphics, FPOINT pos, int frame, float angle, bool flip,
	float alpha, float R, float G, float B, float scaleX, float scaleY)
{
	ColorMatrix colorMatrix = {
		R, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, G, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, B, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);


	int col = frame % maxFrameX;
	int row = frame / maxFrameX;
	float srcX = (float)(col * frameWidth);
	float srcY = (float)(row * frameHeight);
	float destW = frameWidth * scaleX;
	float destH = frameHeight * scaleY;
	Gdiplus::Matrix old;
	graphics->GetTransform(&old);
	Gdiplus::Matrix matrix;
	matrix.RotateAt(angle, Gdiplus::PointF(pos.x + destW *0.5f, pos.y + destH * 0.5f));
	graphics->SetTransform(&matrix);
	
	Gdiplus::Rect destRect(
		(pos.x + (flip ? destW : 0)),
		pos.y,
		(flip ? -destW : destW),
		destH/2
	);

	graphics->DrawImage(
		pBitmap,
		destRect,
		srcX, srcY,
		frameWidth, frameHeight/2,
		UnitPixel,
		&imgAttr
	);
	graphics->SetTransform(&old);
}

void GPImage::Middle_RenderAll(Gdiplus::Graphics* graphics, FPOINT pos, int frame, float angle, bool flip, 
	float alpha, float R, float G, float B, float scaleX, float scaleY)
{
	ColorMatrix colorMatrix = {
		R, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, G, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, B, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);


	int col = frame % maxFrameX;
	int row = frame / maxFrameX;
	float srcX = (float)(col * frameWidth);
	float srcY = (float)(row * frameHeight);
	float destW = frameWidth * scaleX;
	float destH = frameHeight * scaleY;
	Gdiplus::Matrix old;
	graphics->GetTransform(&old);
	Gdiplus::Matrix matrix;
	matrix.RotateAt(angle, Gdiplus::PointF(pos.x, pos.y));
	graphics->SetTransform(&matrix);
	
	Gdiplus::Rect dest(
		(INT)(pos.x - destW * 0.5f + (flip ? destW : 0)),
		(INT)(pos.y - destH * 0.5f),
		(INT)(flip ? -destW : destW),
		(INT)destH);

	graphics->DrawImage(
		pBitmap,
		dest,
		srcX, srcY,
		frameWidth, frameHeight,
		Gdiplus::UnitPixel,
		&imgAttr);
	graphics->SetTransform(&old);
}

void GPImage::Release()
{
	if (pBitmap)
	{
		delete pBitmap;
		pBitmap = nullptr;
	}
	
}

void GPImage::ReleaseLast()
{
	for ( int i = 0; i < 5; i++ )
	{
		delete pens[i];
	}
}

#ifdef _DEBUG
#define new DBG_NEW
#endif