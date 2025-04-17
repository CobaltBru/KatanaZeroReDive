#include "SpriteAnimator.h"
#include "Image.h"


void SpriteAnimator::Init(Image* image, int frames, float totalTime, bool IsLoop)
{
	this->image = image;
	maxFrameIndex = frames;
	deltaFrame = totalTime / float(frames);
	loop = IsLoop;	

	currentFrame = 0;
	frameTimer = 0.0f;
}

void SpriteAnimator::Update(float deltaTime)
{
	if (loop == false && currentFrame == maxFrameIndex) return;

	frameTimer += deltaTime;
	
	while (frameTimer > deltaFrame)
	{
		currentFrame++;
		frameTimer -= deltaTime;

		if (currentFrame >= maxFrameIndex && loop)		
			currentFrame = 0;		
	}
}


