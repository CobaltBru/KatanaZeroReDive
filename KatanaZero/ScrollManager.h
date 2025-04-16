#pragma once
#include "Singleton.h"
#include "config.h"

class ScrollManager : public Singleton<ScrollManager>
{
public:
	ScrollManager();

	void Init();
	void Update();
	void Release();

	FPOINT GetScroll() const { return Scroll; }
	void SetScroll(FPOINT InScroll) {
		ScrollOffset.x += InScroll.x;
		ScrollOffset.y += InScroll.y;
	}

	void ZeroScroll() { 
		ScrollOffset.x = 0.f;
		ScrollOffset.y = 0.f;
	}

	void ZeroScrollX() { ScrollOffset.x = 0.f; }
	void ZeroScrollY() { ScrollOffset.y = 0.f; }

	// 쉐이크 기능
	void CameraShake(float InIntensity, float InSeconds) {
		Intensity = InIntensity;
		ShakeSeconds = InSeconds;
	};
	// 확대 기능&
	void SetFocus(bool InbFocus) { bFocus = InbFocus; }
	bool IsFocus() const { return bFocus; }
private:
	FPOINT Scroll;
	FPOINT ScrollOffset;
	float Speed;
	bool bFocus;

	FPOINT ShakeOffset;
	float Intensity;
	float CurrentTime;
	float ShakeSeconds;
};

