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
	
	//�Ϲ� ���
	void Render(Gdiplus::Graphics* graphics, FPOINT pos);
	void Middle_Render(Gdiplus::Graphics* graphics, FPOINT pos);
	//�ø� + ����
	void Render(Gdiplus::Graphics* graphics, FPOINT pos,bool flip, float alpha = 1.0f);
	void Middle_Render(Gdiplus::Graphics* graphics, FPOINT pos, bool flip, float alpha = 1.0f);
	//�ø� + ���� + �ִϸ��̼�
	void RenderFrame(Gdiplus::Graphics* graphics, FPOINT pos, int frame=0, bool flip = false, float alpha = 1.0f);
	void Middle_RenderFrame(Gdiplus::Graphics* graphics, FPOINT pos, int frame=0, bool flip = false, float alpha = 1.0f);

	//�ø� + ���� + �ִϸ��̼� + ȸ��
	void RenderFrameAngle(Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, float angle = 0.f, bool flip = false, float alpha = 1.0f);
	void Middle_RenderFrameAngle(Gdiplus::Graphics* graphics, FPOINT pos, int frame = 0, float angle = 0.f, bool flip = false, float alpha = 1.0f);


	inline int getWidth() { return width; }
	inline int getHeight() { return height; }
	
	void Release();
	static void ReleaseLast();
};

