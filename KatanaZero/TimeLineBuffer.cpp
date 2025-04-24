#include "TimeLineBuffer.h"

void TimeLineBuffer::Init()
{
}

void TimeLineBuffer::Release()
{
	
}

void TimeLineBuffer::SaveSnapShot(const PlayerSnapShot& player, const vector<EnemySnapShot>& enemies, const ScrollSnapShot& scroll)
{
	SnapShot currFrame;
	currFrame.player = player;
	currFrame.scroll = scroll;

	frameBuffer.push_back(currFrame);
	if (frameBuffer.size() > MAX_SNAPSHOTS)
	{
		frameBuffer.pop_front();
	}
	
}

void TimeLineBuffer::SaveFx(const vector<EffectSnapShot>& effects)
{
	fxBuffer.push_back(effects);
}
