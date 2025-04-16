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


class Chat
{
protected:
	FPOINT pos;
	vector <pair<float, Token >> tokens;
	int tokenIdx;
	//��ǳ�� ũ��
	float width;
	float height;
	float timer;
	//��ǳ�� �����µ� �ɸ��� �ð�
	float boxTime;

	int statusFlag = 0;
public:
	void Init(vector <pair<float, Token >> tokens, float width, float height);
	inline void setPos(FPOINT pos) { this->pos = pos; }
	virtual void Update();
	virtual void Render(HDC hdc);
	void DrawBox(HDC hdc);
	void DrawTokens(HDC hdc);
	void makeExplode();
	inline int getStatus() { return statusFlag; }
	virtual void moveCursor(int way) {};
	virtual inline string selectCursor() { return "NULL"; }
};

enum class OptionState { RED, TRANSITION, NORMAL };
class OptionChat : public Chat
{
public:

private:
	vector<pair<string, Token>> redSelects;
	vector<pair<string, Token>> normalSelects;

	OptionState state;

	float animDuration;    // ����/���� �ִϸ��̼� ���ӽð�

	//���� ����
	int cursor;
	//�����ð� ����
	float redTime;
	//��ü�ð� ����
	float totalTime;
	float timer;

	//�ð� �� ��ġ, ����
	FPOINT timeBarPos;
	float timeBarWidth;

	//1�� ������ ��ġ
	FPOINT selectBarPos;
	float currentSelectBarX;
	float selectWidth;
	float selectHeight;
	//�������� ���� (1�� �Ʒ��� �ڵ� ��ġ)
	float selectGap;
public:
	void Init(vector <pair<float, Token >> tokens, float width, float height, 
		float redTime, float totalTime, 
		vector<pair<string, Token>> redSelects,
		vector<pair<string, Token>> normalSelects);
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void DrawTimeBar(HDC hdc);
	void DrawSelects(HDC hdc);

	void DrawRedSelects(HDC hdc);
	void DrawNormalSelects(HDC hdc);
	virtual void moveCursor(int way) override;
	virtual string selectCursor() override;

};
class ChatManager : public GameObject
{
private:
	vector<FPOINT> poses;
	map<string, pair<Chat*,string>> chatMap;
	Chat* currentChat = nullptr;
	string nextChat;
	bool explodeFlag = false;
	string tmpChat;
	float timer = 0;


public:
	inline void pushPos(FPOINT pos) { poses.push_back(pos); }
	void Push(string key, string next,int pos, Chat* chat);
	void startChat(string key);
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

