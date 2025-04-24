#pragma once
#include "config.h"

struct ObsPlayerState
{
	FPOINT pos;
	float battery;
	std::string leftItem;
	std::string rightItem;
	bool isLive;
	bool isSlow;
};

class Observer
{
public:
	virtual void EventPlayerState(const ObsPlayerState& ps) {};
	virtual void TimerUIEvent(const float t) {};

	//�ѱ�, ����Ű, ȣ���Ҿ�����, ������ ��ġ, ��ũ��
	//virtual void PopUpIconEvent(bool trigger, string code, string key, FPOINT* pos, bool flag);
};

