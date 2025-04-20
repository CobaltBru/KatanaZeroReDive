#pragma once
#include "config.h"
#include <array>

enum MoveFlags {
	Move_Linear = 0,
	Move_SoftStart = 1 << 0,
	Move_SoftEnd = 1 << 1,
	Move_Loop = 1 << 2,
	Move_Stop = 1 << 3
};

struct Task
{
	FPOINT src;
	FPOINT dest;
	FPOINT offset;
	float duration;
	float timer;

	int flag;
};

class Image;
class Animation
{
private:
	Image* image;
	FPOINT pos;
	//idx, time
	std::vector<pair<int,float>> aniTasks;
	Task moveTask;
	//�� ������ �� ����
	int frameX;

	//���� ������� �ִ� �׽�ũ&���� ������
	int anitaskIdx;
	int frameIdx;

	bool flip;
	//false = �»��, true = �߾�
	bool anker;
	//Sour ����, ��
	float sStart;
	float sEnd;

	float timer;
	bool isStart;
	bool isOn;
	bool isMove;

public:
	void Init(Image* image, int frameX);
	void Update();
	void Render(HDC hdc);

	void Start();
	void Stop();
	void On();
	void Off();
	void MoveOn(FPOINT dest, float duration, int flag);
	void MoveOff();

	void setPos(FPOINT pos, bool flip, bool anker = false);
	void setSour(float start, float end);
	inline void setFrame(int idx) { this->frameIdx = idx; }
	void setAniTask(initializer_list<pair<int, float>> lst);
	void setAniTask(std::vector < pair<int, float>>& lst);
};

