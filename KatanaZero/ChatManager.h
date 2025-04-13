#pragma once
#include "GameObject.h"
class Token : public GameObject
{
public:
	enum APPEAR { NORMAL, DOOM, END };
	enum OPTION { STOP, SHAKE, WAVE};
	enum COLORS { WHITE, RED, YELLOW, PURPLE, ORANGE, GREEN, SKY };
private:
	
	FPOINT pos;
	const wchar_t* text;
	int len;
	APPEAR appear;
	OPTION option;
	COLORS color;
	COLORREF savedColor;
	float timer;
public:
	Token(const wchar_t* text,APPEAR appear, OPTION option, COLORS color);
	inline void setPos(FPOINT pos) { this->pos = pos; }
	void Update();
	void Render(HDC hdc);

	void NormalAppear(HDC hdc);
	void DoomAppear(HDC hdc);

	void WaveEffect(HDC hdc);
	void ShakeEffect(HDC hdc);
};
class Chat
{
private:
	string key;
	FPOINT pos;
	vector <pair<float, Token >> tokens;
	float width;
	float timer;
	float delay;
public:
	void Update();
	void Render(HDC hdc);
};
class ChatManager : public GameObject
{


};

