#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "Pawn.h"
#include "PlayerInput.h"
#include "PlayerState.h"
#include <stack>


class Image;
class Player;
class PlayerAnim;
class PlayerInput;
class Collider;
class RigidBody;
class AttackState;
class FallState;

struct stateAnimFunc
{
	SpriteAnimator* animator;
	std::function<void(Player&, EDirection)> func;
};

struct playerInfo
{
	bool bIsAttack;
	bool bIsJump;
	bool bIsFlip;
	bool bIsShift;
	bool bIsShiftChanged;
};

struct playerStates
{
	PlayerState* Idle;
	PlayerState* Attack;
	PlayerState* Fall;
	PlayerState* Run;
	PlayerState* Jump;
	PlayerState* Flip;
	PlayerState* WallSlide;
};

class Player: public Pawn
{
private:
	PlayerInput* playerInput;
	EDirection dir;

	stateAnimFunc playerAnimFunc;

	bool bIsLeft;
	bool bWall;

	float scrollSpeed;

	// player EState
	playerStates* states;
	PlayerState* state;
	EPlayerState EState;

	// player state에 따른 move function과 animation을 따로 관리
	
	// player anim
	// PlayerAnim* playerAnim;
	Image* image;
	Image* effectImage;

	playerInfo* info;

	// switch frame
	float switchTime;	

	Collider* AttackCollider;

public:
	Player();
	virtual ~Player();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

	void InitImage();
	 
	void InitPlayerStates();
	void InitPlayerInfo();

	// rigid body
	void InitRigidBody();
	void UpdateRigidBody();

	void UpdateCollision();

	// scroll
	void InitScrollOffset();
	void Offset();


	inline playerStates* GetStates() { return states; }
	inline playerInfo* GetInfo() { return info; }

	inline Image* GetImage() { return image; }
	inline void SetImage(Image* image) { this->image = image; }

	inline void SetEffectImage(Image* image) { effectImage = image; }

	inline EPlayerState GetEState() { return EState; }
	inline void SetEState(EPlayerState state) { this->EState = state; }

	/*inline PlayerState* GetState() { return state; }
	inline void SetState(PlayerState* state) { this->state = state; }*/

	inline EDirection GetDirection() { return dir; };
	inline void SetDirection(EDirection dir) { this->dir = dir; }

	inline void SetSwitchTime(float time) { switchTime = time; }

	inline Collider* GetAttackCollider() { return AttackCollider; }
	void InitBindState();
};


	//typedef std::function<void(Player&)> stateFunction;
	/*
	typedef std::function<void(Player&, EDirection)> stateFunction;

	// FSM: input, function binding
	std::unordered_map<EInputAction, stateFunction> inputStateMap;

	std::unordered_map<EInputAction, EPlayerState> ipActionPlayerStateMap;
	std::unordered_map<EPlayerState, stateAnimFunc> playerStateFunctionMap;	

	stateAnimFunc IdleAnimFunc;
	stateAnimFunc IdleToRunAnimFunc;
	stateAnimFunc RunToIdleAnimFunc;	
	stateAnimFunc RunAnimFunc;
	stateAnimFunc FlipAnimFunc;
	stateAnimFunc JumpAnimFunc;
	stateAnimFunc AttackAnimFunc;
	*/