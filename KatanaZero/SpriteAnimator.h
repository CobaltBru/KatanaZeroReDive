#pragma once


class Image;
class SpriteAnimator
{
private:
	Image* image;

	int currentFrameIndex;
	int maxFrameIndex;

	float deltaFrameThreshold;
	float frameTimer;
	 
	bool loop;
	bool finished;

public:
	void Init(Image* image, int frames, bool IsLoop = true, float totalTime = 0.0f);
	void InitFrame() { currentFrameIndex = 0; }

	Image* GetImage() { return image; }
	int GetCurrentFrameIndex() { return currentFrameIndex; }

	bool UpdateFrame(float deltaTime);
};

