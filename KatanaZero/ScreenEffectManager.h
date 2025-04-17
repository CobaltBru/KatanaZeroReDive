#pragma once
#include "Singleton.h"
#include "config.h"

class GPImage;
class ScreenEffectManager : public Singleton<ScreenEffectManager>
{
public:
	void Init();
	void Release();
	void StartDistortion();
	void RenderDistortion(HDC hdc);
	void RenderNoise(HDC hdc);
	void StopDistortion();
private:
	bool isDistortion{ false };
	float elapsedTime{ 0.0f };
	float distortionForce{ 7.0f };
	float distortionSpeed{ 200.0f };
	int glitchOffset{ 0 };
	GPImage* noise;
};

