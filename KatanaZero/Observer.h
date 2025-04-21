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

	//�ѱ�, ����Ű, ȣ���Ҿ�����, ������ ��ġ, ��ũ��
	//virtual void PopUpIconEvent(bool trigger, string code, string key, FPOINT* pos, bool flag);
};

