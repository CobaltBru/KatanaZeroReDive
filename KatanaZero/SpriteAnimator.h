#pragma once


class Image;
class SpriteAnimator
{
private:
	Image* image;

	int currentFrame;
	int maxFrameIndex;

	float deltaFrame;
	float frameTimer;
	 
	bool loop;
	bool finished;

public:
	void Init(Image* image, int frames, float totalTime, bool IsLoop);

	void Update(float deltaTime);
};

