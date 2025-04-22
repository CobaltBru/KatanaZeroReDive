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
	FPOINT GetScrollOffset() const { return ScrollOffset; }
	float GetScale() const { return Scale; }

	void SetScroll(FPOINT InScroll) {
		ScrollOffset.x += InScroll.x;
		ScrollOffset.y += InScroll.y;
	}

	void ReplayScroll(FPOINT prevScroll)
	{
		Scroll = prevScroll;
	}

	void ZeroScroll() { 
		ScrollOffset.x = 0.f;
		ScrollOffset.y = 0.f;
	}

	void ZeroScrollX() { ScrollOffset.x = 0.f; }
	void ZeroScrollY() { ScrollOffset.y = 0.f; }

	// ����ũ ���
	void CameraShake(float InIntensity, float InSeconds) {
		Intensity = InIntensity;
		ShakeSeconds = InSeconds;
	};
	// TODO : Ȯ�� ��� ��������

	void SetFocus(bool InbFocus) { bFocus = InbFocus; }
	bool IsFocus() const { return bFocus; }

	void ScaleUpdate();
private:
	FPOINT Scroll;
	FPOINT ScrollOffset;
	float Speed;
	bool bFocus;

	FPOINT ShakeOffset;
	float Intensity;
	float CurrentTime;
	float ShakeSeconds;

	float Scale;
};

