#pragma once
#include "GameObject.h"
#include "PlayerState.h"

class SpriteAnimator;
class PlayerAnim: public GameObject
{
private:
	SpriteAnimator* idleAnim;
	SpriteAnimator* idleToRunAnim;
	SpriteAnimator* runToIdleAnim;
	SpriteAnimator* runAnim;
	SpriteAnimator* flipAnim;
	SpriteAnimator* walkAnim;
	SpriteAnimator* crouchAnim;
	SpriteAnimator* jumpAnim;
	SpriteAnimator* attackAnim;

	float idleToRunThreshold;

	static const int STATE_COUNT = static_cast<int>(EPlayerState::End);
	bool stateTransitionTable[STATE_COUNT][STATE_COUNT] = {false};

public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void InitStateTransitionTable();
	bool GetStateTransitionTable() { return stateTransitionTable; }

	float GetIdleToRunSpeed() { return idleToRunThreshold; }

	SpriteAnimator* GetIdleAnim() { return idleAnim; }
	SpriteAnimator* GetIdleToRunAnim() { return idleToRunAnim; }
	SpriteAnimator* GetRunToIdleAnim() { return runToIdleAnim; }
	SpriteAnimator* GetRunAnim() { return runAnim; }
	SpriteAnimator* GetFlipAnim() { return flipAnim; }
	SpriteAnimator* GetWalkAnim() { return walkAnim; }
	SpriteAnimator* GetDownAnim() { return crouchAnim; }
	SpriteAnimator* GetJumpAnim() { return jumpAnim; }
	SpriteAnimator* GetAttackAnim() { return attackAnim; }
};

/*
in order natual: Idle -> IdleToRun -> Run -> RunToIdle -> Idle
should be in order: Run -> Flip
together: Jump - Run
exclusive: Attack
*/