#pragma once
#include "GameObject.h"
class Token : public GameObject
{
public:
	enum OPTION { NORMAL, SHAKE, WAVE, DOOM };
	enum COLORS { WHITE, RED, YELLOW, PURPLE, ORANGE };
private:
	
	FPOINT pos;
	float left, right;
	const wchar_t* text;
	OPTION option;
	COLORS color;
	
	COLORREF savedColor;
	float timer;
public:
	Token(const wchar_t* text, OPTION option, COLORS color);
	inline void setPos(FPOINT pos) { this->pos = pos; }
	void Update();
	void Render(HDC hdc);
	void WaveEffect(HDC hdc);
};

class ChatManager : public GameObject
{


};

