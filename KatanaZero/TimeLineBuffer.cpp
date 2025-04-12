#include "TimeLineBuffer.h"

void TimeLineBuffer::Init()
{
}

void TimeLineBuffer::Release()
{
	
}

void TimeLineBuffer::SaveSnapShot(const PlayerSnapShot& player, const vector<EnemySnapShot>& enemies, const vector<EffectSnapShot>& effects)
{
	SnapShot currFrame;
	currFrame.player = player;
	currFrame.enemies = enemies;
	currFrame.effects = effects;

	frameBuffer.push_back(currFrame);
	if (frameBuffer.size() > MAX_SNAPSHOTS)
	{
		frameBuffer.pop_front();
	}
}

void TimeLineBuffer::Replay()
{

}
