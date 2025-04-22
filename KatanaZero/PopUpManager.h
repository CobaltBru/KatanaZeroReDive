#pragma once
#include "config.h"
#include "GameObject.h"
#include "Observer.h"


class Animation;

struct PopUp
{
	Animation* ani;
	FPOINT* pos;
	bool flag;
};

class PopUpManager :public GameObject, public Observer
{
private:
	map<string, Animation*> mp;
	vector<pair<string, PopUp>> activePopUp;



public:
	virtual ~PopUpManager() = default;

	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	//virtual void PopUpIconEvent(bool trigger, string code, string key, FPOINT* pos, bool flag) override;
	virtual void Release() override;
	void TurnOnPopUp(pair<string, PopUp>& p);
	void TurnOffPopUp(string code);

};

