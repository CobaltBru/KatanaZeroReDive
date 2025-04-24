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
		const ScrollSnapShot& scroll);
	void SaveFx(const vector<EffectSnapShot>& effects);
	inline int GetBufferSize() { return (int)frameBuffer.size(); }
	inline int GetFxBufferSize() { return (int)fxBuffer.size(); }
	inline SnapShot GetFrame(int idx) { return frameBuffer[idx]; }
	inline vector<EffectSnapShot> GetFxFrame(int idx) { return fxBuffer[idx]; }
	inline void Clear() 
	{ 
		frameBuffer.clear();
		fxBuffer.clear();
	}

private:
	const size_t MAX_SNAPSHOTS = 100000; // 180fps로 저장하는데 
	deque<SnapShot> frameBuffer;
	deque<vector<EffectSnapShot>> fxBuffer;
};

