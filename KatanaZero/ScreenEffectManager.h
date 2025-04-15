#pragma once
#include "Singleton.h"
#include "config.h"
class ScreenEffectManager : public Singleton<ScreenEffectManager>
{
public:
	void StartDistortion();
	void RenderDistortion(HDC hdc);
	void RenderGlitch(HDC hdc);
	void StopDistortion();
private:
	bool isDistortion{ false };
	float elapsedTime{ 0.0f };
	float distortionForce{ 7.0f };
	float distortionSpeed{ 20.0f };
	int glitchOffset{ 0 };
};

