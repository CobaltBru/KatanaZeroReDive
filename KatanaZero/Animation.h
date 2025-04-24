#pragma once
#include "config.h"
#include "GameObject.h"
#include <array>

enum MoveFlags {
	Move_Linear = 0,
	Move_SoftStart = 1 << 0,
	Move_SoftEnd = 1 << 1,
	Move_Loop = 1 << 2,
	Move_Stop = 1 << 3,
	POS_Update = 1 << 4
	
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
class GPImage;
class Animation : public GameObject
{
private:
	bool imageFlag;
	Image* image;
	GPImage* gpimage;
	FPOINT pos;
	float scale;
	//idx, time
	std::vector<pair<int,float>> aniTasks;
	std::vector<pair<string, int>> soundTasks;
	Task moveTask;
	//총 프레임 수 저장
	int frameX;

	//현재 재생중인 애니 테스크&현재 프레임
	int anitaskIdx;
	int frameIdx;

	bool flip;
	//false = 좌상단, true = 중앙
	bool anker;
	//Sour 시작, 끝
	float sStart;
	float sEnd;

	float timer;
	bool isStart;
	bool isOn;
	bool isMove;
	
	bool isMoveComplete;
	bool isComplete;
	bool loopflag;
public:
	virtual ~Animation() = default;
	void Init(Image* image, int frameX , float scale = 1.0f);
	void Init(GPImage* image, int frameX , float scale = 1.0f);
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

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

	void setSoundTask(initializer_list<pair<string, int>> lst);
	virtual void Release() override;
	void setloopFlag(bool flag);

	inline bool getAniComplete() { return isComplete; }
	inline bool getMoveComplete() { return isMoveComplete; }
	inline FPOINT getCurrentPos() { return { pos.x + moveTask.offset.x,pos.y + moveTask.offset.y }; }
};

