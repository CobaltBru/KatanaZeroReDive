#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "Pawn.h"
#include "PlayerInput.h"
#include "PlayerState.h"

class Image;
class Player;
class PlayerAnim;
struct PlayerStateInfo
{
	SpriteAnimator* animator;
	std::function<void(Player&)> func;
};

class Collider;
class PlayerInput;
class Player: public Pawn
{
private:
	Collider* playerCollider;
	 
	PlayerInput* playerInput;
	EPlayerState playerState;
	PlayerAnim* playerAnim;

	// movemnet physics
	FPOINT velocity;
	FPOINT accel;
	FPOINT addAccel;

	// switch frame
	float switchTime;	

	typedef std::function<void(Player&)> stateFunction;

	// FSM: input, function binding
	std::unordered_map<EInputAction, stateFunction> inputStateMap;

	std::unordered_map<EInputAction, EPlayerState> ipActionplayerStateMap;
	std::unordered_map<EPlayerState, PlayerStateInfo> playerStateFunctionMap;

	EPlayerState newState;
	EPlayerState currentState;

public:
	Player();
	virtual ~Player();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

	void InitBindState();

	void Left();
	void Right();
	void Down();
	void Jump();
	void Fall();
	void Attack();
	void Dead();
	void WallSlide();
};

