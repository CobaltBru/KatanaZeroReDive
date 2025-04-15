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
		Scroll.x += InScroll.x;
		Scroll.y += InScroll.y;
	}

	void ZeroScroll() { 
		Scroll.x = 0.f;
		Scroll.y = 0.f;
	}

	void ZeroScrollX() { Scroll.x = 0.f; }
	void ZeroScrollY() { Scroll.y = 0.f; }

	// 쉐이크 기능
	// 확대 기능&
	void SetFocus(bool InbFocus) { bFocus = InbFocus; }
	bool IsFocus() const { return bFocus; }
private:
	FPOINT Scroll;
	float Speed;
	bool bFocus;

};

