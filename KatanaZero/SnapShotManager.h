#pragma once
#include "Singleton.h"
#include "TimeLineBuffer.h"
class SnapShotManager : public Singleton<SnapShotManager>
{
private:
	TimeLineBuffer* snapShots;
public:
	void Init();
	void Release();
	void Save();
	void Replay();
};

