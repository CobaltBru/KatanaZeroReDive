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

struct stateAnimFunc
{
	SpriteAnimator* animator;
	std::function<void(Player&, EDirection)> func;
};

class Player: public Pawn
{
private:
	PlayerInput* playerInput;
	EDirection dir;

	EPlayerState currPlayerState;
	stateAnimFunc playerAnimFunc;
	EPlayerState prevPlayerState;
	EPlayerState newState;

	// rigid body
	void InitRigidBody();

	// scroll
	void InitScrollOffset();
	void Offset();
	float scrollSpeed;

	// player state
	std::stack<EPlayerState> PlayerStateStack;
	std::vector<EPlayerState> newPlayerStates;

	// player state에 따른 move function과 animation을 따로 관리
	
	// player anim
	PlayerAnim* playerAnim;

	bool bIsLeft;
	bool bWall;

	// switch frame
	float switchTime;	

	//typedef std::function<void(Player&)> stateFunction;
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
public:
	Player();
	virtual ~Player();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

	void InitBindState();

	void Idle(EDirection dir);
	void Run(EDirection dir);
	void Walk(EDirection dir);
	void Flip(EDirection dir);
	void Down(EDirection dir);
	void Jump(EDirection dir);
	void Fall(EDirection dir);
	void Attack(EDirection dir);
	void WallSlide(EDirection dir);	
};

