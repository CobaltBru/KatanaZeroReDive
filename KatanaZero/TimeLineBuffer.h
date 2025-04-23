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
		const vector<EffectSnapShot>& effects, 
		const ScrollSnapShot& scroll);
	inline int GetBufferSize() { return (int)frameBuffer.size(); }
	inline SnapShot GetFrame(int idx) { return frameBuffer[idx]; }
	inline void Clear() { frameBuffer.clear(); }

private:
	const size_t MAX_SNAPSHOTS = 10000;
	deque<SnapShot> frameBuffer;
};

