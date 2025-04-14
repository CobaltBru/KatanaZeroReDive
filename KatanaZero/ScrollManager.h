#pragma once
#include "Singleton.h"
#include "config.h"

class ScrollManager : public  Singleton<ScrollManager>
{
public:
	void Init();

	FPOINT GetScroll() const { return Scroll; }
	void SetScroll(FPOINT InScroll) {
		Scroll.x += InScroll.x;
		Scroll.y += InScroll.y;
	}

	void ReplayScroll(FPOINT prevScroll)
	{
		Scroll = prevScroll;
	}

	void ZeroScroll() { 
		Scroll.x = 0.f;
		Scroll.y = 0.f;
	}

	void ZeroScrollX() { Scroll.x = 0.f; }
	void ZeroScrollY() { Scroll.y = 0.f; }

private:
	FPOINT Scroll;

};

