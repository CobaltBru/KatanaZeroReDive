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
	void StopDistortion();
	void Distortion(HDC hdc, float frequency,float distortionSpeed, float distortionForce);
	void ScrollDistortion(HDC hdc, float frequency, float distortionSpeed, float distortionForce);
private:
	bool isDistortion{ false };
	bool isFinishReplay{ false };

	//마지막 스크롤 효과
	float scrollTimer{ 0.0f };
	float scrollOffset{ 0.0f };
	float scrollSpeed{ WINSIZE_Y * 4.0f };
	float scrollacceleration{ 1.3f };
	float scrollDuration{ 1.0f };

	//왜곡 강도
	float elapsedTime{ 0.0f };
	float distortionForce{ 7.0f };
	float distortionSpeed{ 100.0f };
	float frequency{ 0.06f };
	GPImage* noise;
};

