#pragma once



class SpriteAnimator;
class PlayerAnim
{
private:
	SpriteAnimator* IdleAnim;
	SpriteAnimator* IdleToRunAnim;
	SpriteAnimator* RunToIdleAnim;
	SpriteAnimator* WalkAnim;
	SpriteAnimator* DownAnim;
	SpriteAnimator* JumpAnim;
	SpriteAnimator* AttackAnim;

public:
	void Init();

	SpriteAnimator* GetIdleAnim() { return IdleAnim; }
	SpriteAnimator* GetIdleToRunAnim() { return IdleToRunAnim; }
	SpriteAnimator* GetRunToIdleAnim() { return RunToIdleAnim; }
	SpriteAnimator* GetWalkAnim() { return WalkAnim; }
	SpriteAnimator* GetDownAnim() { return DownAnim; }
	SpriteAnimator* GetJumpAnim() { return JumpAnim; }
	SpriteAnimator* GetAttackAnim() { return AttackAnim; }
};

