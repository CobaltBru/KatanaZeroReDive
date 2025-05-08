#pragma once
#include "config.h"

class Image;
class GPImage;
class SpriteAnimation
{
private:
	//true == gpImage, false == image
	bool ImageType;
	Image* image;
	GPImage* gpImage;

	FPOINT pos;
	bool flip;

	int maxFrameX;
	int currentFrame;
	float frameTime;

	float timer;

	bool isLoop;
	bool isEffect;

public:
	void Init(Image* image, float frameTime, bool isLoop);
	void Init(GPImage* image, float frameTime, bool isLoop);
	void Update(FPOINT pos,bool flip, bool isEffect);
	void Render(HDC hdc);
	bool isFinish();
	void reset();
	inline GPImage* getGPImage() { return gpImage; }
	inline Image* getImage() { return image; }
	int getCurrFrame() { return currentFrame; }
};

