#pragma once
#include "config.h"
#include <deque>
#include "SnapShot.h"

class TimeLineBuffer
{
public:
	TimeLineBuffer() {};
	~TimeLineBuffer() {};

	void Init();
	void Release();

	void SaveSnapShot(
		const PlayerSnapShot& player,
		const vector<EnemySnapShot>& enemies,
		const vector<EffectSnapShot>& effects);
	inline int GetBufferSize() { return (int)frameBuffer.size(); }
	inline SnapShot GetFrame(int idx) { return frameBuffer[idx]; }

private:
	const size_t MAX_SNAPSHOTS = 300;
	deque<SnapShot> frameBuffer;
};

