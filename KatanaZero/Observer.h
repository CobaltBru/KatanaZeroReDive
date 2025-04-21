#pragma once
#include "config.h"

struct PlayerState
{
	FPOINT pos;
	float battery;
	std::string leftItem;
	std::string rightItem;
	bool isLive;

};

class Observer
{
public:
	virtual void EventPlayerState(const PlayerState& ps) {};
	virtual void TimerUIEvent(const float t) {};

	//켜기, 고유키, 호출할아이콘, 연동할 위치, 스크롤
	//virtual void PopUpIconEvent(bool trigger, string code, string key, FPOINT* pos, bool flag);
};

