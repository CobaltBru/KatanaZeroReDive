#include "SpriteAnimator.h"
#include "Image.h"


void SpriteAnimator::Init(Image* image, int frames, bool IsLoop, float totalTime)
{
	if (totalTime == 0.0f)
	{
		totalTime = frames * 0.1f;
	}
	this->image = image;
	maxFrameIndex = frames;
	deltaFrameThreshold = totalTime / float(frames);
	loop = IsLoop;	

	currentFrameIndex = 0;
	frameTimer = 0.0f;
}

// Update frameIndex
bool SpriteAnimator::UpdateFrame(float deltaTime)
{
	if (loop == false && currentFrameIndex >= maxFrameIndex)
	{	
		currentFrameIndex = 0;
		frameTimer = 0.0f;
		return false;
	}

	frameTimer += deltaTime;
	
	/*while (frameTimer > deltaFrameThreshold)
	{
		currentFrameIndex++;
		frameTimer -= deltaFrameThreshold;

		if (currentFrameIndex >= maxFrameIndex && loop)		
			currentFrameIndex = 0;		
	}*/

	if (frameTimer > deltaFrameThreshold)
	{
		currentFrameIndex++;
		frameTimer = 0.0f;

		if (currentFrameIndex >= maxFrameIndex && loop)
			currentFrameIndex = 0;
	}


	return true;
}


