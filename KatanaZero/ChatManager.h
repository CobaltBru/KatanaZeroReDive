#pragma once
#include "GameObject.h"
class Token
{
public:
	enum APPEAR { NORMAL, DOOM, END };
	enum OPTION { STOP, SHAKE, WAVE};
	enum COLORS { WHITE, RED, YELLOW, PURPLE, ORANGE, GREEN, SKY };
private:
	FPOINT globalPos;
	FPOINT pos;
	const wchar_t* text;
	int len;
	APPEAR appear;
	OPTION option;
	COLORS color;
	COLORREF savedColor;
	float timer;

	bool complete;
public:
	Token(const wchar_t* text,FPOINT pos, APPEAR appear, OPTION option, COLORS color);
	inline void setGlobalPos(FPOINT pos) { this->globalPos = pos; }
	inline void setPos(FPOINT pos) { this->pos = pos; }
	void Update();
	void Render(HDC hdc);

	void NormalAppear(HDC hdc);
	void DoomAppear(HDC hdc);

	void WaveEffect(HDC hdc);
	void ShakeEffect(HDC hdc);

	inline bool isComplete() { return complete; }
	inline int getLen() { return len; }
};


class Chat : public GameObject
{
protected:
	string key;
	FPOINT pos;
	vector <pair<float, Token >> tokens;
	int tokenIdx;
	float width;
	float height;
	float timer;
	float delay;
	float boxTime;
public:
	void Init(string Key, vector <pair<float, Token >> tokens, float width, float height);
	inline void setPos(FPOINT pos) { this->pos = pos; }
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void DrawBox(HDC hdc);
	void DrawTokens(HDC hdc);
};

class OptionChat : public Chat
{
public:
	void Update();
	void Render(HDC hdc);
};
class ChatManager : public GameObject
{


};

