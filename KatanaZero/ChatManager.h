#pragma once
#include "GameObject.h"
class Token
{
public:
	enum APPEAR { NORMAL, DOOM, EXPLODE, END };
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

	float explodeTimer;
	int currentAppear;
	bool complete;
public:
	Token(const wchar_t* text,FPOINT pos, APPEAR appear, OPTION option, COLORS color);
	inline void setGlobalPos(FPOINT pos) { this->globalPos = pos; }
	inline void setPos(FPOINT pos) { this->pos = pos; }
	void Update();
	void Render(HDC hdc);

	void NormalAppear(HDC hdc);
	void DoomAppear(HDC hdc);
	void ExplodeAppear(HDC hdc);

	void WaveEffect(HDC hdc);
	void ShakeEffect(HDC hdc);

	inline void setExplode() { appear = APPEAR::EXPLODE; }
	inline bool isComplete() { return complete; }
	inline int getLen() { return len; }
	inline SIZE getSize(HDC hdc) 
	{
		SIZE size;
		GetTextExtentPoint32W(hdc, text, len, &size);
		return size;
	}
};


class Chat : public GameObject
{
protected:
	FPOINT pos;
	vector <pair<float, Token >> tokens;
	int tokenIdx;
	//말풍선 크기
	float width;
	float height;
	float timer;
	//말풍선 켜지는데 걸리는 시간
	float boxTime;

	int statusFlag = 0;
public:
	void Init(vector <pair<float, Token >> tokens, float width, float height);
	inline void setPos(FPOINT pos) { this->pos = pos; }
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void DrawBox(HDC hdc);
	void DrawTokens(HDC hdc);
	void makeExplode();
	inline int getStatus() { return statusFlag; }
	virtual inline string getNext() { return "NULL"; }
};

class OptionChat : public Chat
{
private:
	vector<pair<string, Token>> selects;
	//현재 선택
	int cursor;
	//빨간시간 길이
	float redTime;
	//전체시간 길이
	float totalTime;
	float timer;

	//시간 바 위치, 길이
	FPOINT timeBarPos;
	float timeBarWidth;

	//1번 선택지 위치
	FPOINT selectBarPos;
	float selectWidth;
	float selectHeight;
	//선택지간 간격 (1번 아래로 자동 배치)
	float selectGap;
public:
	void Init(vector <pair<float, Token >> tokens, float width, float height, 
		float redTime, float totalTime, vector<pair<string, Token>> selects);
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void DrawTimeBar(HDC hdc);
	void DrawSelects(HDC hdc);
	virtual inline string getNext() override { return selects[cursor].first; }
};
class ChatManager : public GameObject
{
private:
	map<string, pair<Chat*,string>> chatMap;
	Chat* currentChat = nullptr;
	string nextChat;
public:
	void Init(string key, string next,FPOINT pos, Chat* chat);
	void startChat(string key);
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

