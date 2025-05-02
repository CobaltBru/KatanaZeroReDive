#pragma once
#include "config.h"
class GPImage
{
private:
	static Gdiplus::Pen* pens[5];
	Gdiplus::Bitmap* pBitmap;

	int width;
	int height;
	int maxFrameX;
	int maxFrameY;
	int frameWidth;
	int frameHeight;
public:
	enum Pcolor { BLACK , WHITE , RED , GREEN , BLUE };

	static void Init();
	void AddImage(const wchar_t* filePath);
	void AddImage(const wchar_t* filePath, int frameX, int frameY);
	void RenderRect(Gdiplus::Graphics* graphics , FPOINT pos , float width , float height , Pcolor color);
	void Middle_RenderRect(Gdiplus::Graphics* graphics , FPOINT pos , float width , float height , Pcolor color);
	
	//일반 출력
	void Render(Gdiplus::Graphics* graphics, FPOINT pos);
	void Middle_Render(Gdiplus::Graphics* graphics, FPOINT pos);
	//플립 + 알파
	void Render(Gdiplus::Graphics* graphics, FPOINT pos,bool flip, float alpha = 1.0f);
	void Middle_Render(Gdiplus::Graphics* graphics, FPOINT pos, bool flip, float alpha = 1.0f);
	//플립 + 알파 + 애니메이션
	void RenderFrame(Gdiplus::Graphics* graphics, FPOINT pos, int frame=0, bool flip = false, float alpha = 1.0f);
	void Middle_RenderFrame(Gdiplus::Graphics* graphics, FPOINT pos, int frame=0, bool flip = false, float alpha = 1.0f);

	void RenderFrameScale(Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, bool flip = false, float alpha = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	void Middle_RenderFrameScale(Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, bool flip = false, float alpha = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	//플립 + 알파 + 애니메이션 + 회전
	void RenderFrameAngle(Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, float angle = 0.f, bool flip = false, float alpha = 1.0f);
	void Middle_RenderFrameAngle(Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, float angle = 0.f, bool flip = false, float alpha = 1.0f);

	//플립 + 알파 + 애니메이션 + 회전 + RGB + 확대축소
	void RenderAll(
		Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, 
		float angle = 0.f, bool flip = false, 
		float alpha = 1.0f, float R = 1.0f, float G = 1.0f, float B = 1.0f, 
		float scaleX = 1.0f, float scaleY = 1.0f);
	void Middle_RenderAll(
		Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, 
		float angle = 0.f, bool flip = false, 
		float alpha = 1.0f, float R = 1.0f, float G = 1.0f, float B = 1.0f, 
		float scaleX = 1.0f, float scaleY = 1.0f);

	//Sour렌더 추가
	void SourRender(Gdiplus::Graphics* graphics, FPOINT pos, int offset, int frame = 0, bool flip = false, 
		float alpha = 1.0f, float R = 1.0f, float G = 1.0f, float B = 1.0f, float rb = 0.0f, float gb = 0.0f, float bb = 0.0f);

	inline int getWidth() { return width; }
	inline int getHeight() { return height; }
	inline int getFrameWidth() { return frameWidth; }
	inline int getFrameHeight() { return frameHeight; }
	inline int getMaxFrame() { return maxFrameX; }
	
	void Release();
	static void ReleaseLast();

};

