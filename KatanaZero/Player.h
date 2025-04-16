#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "Pawn.h"
#include "PlayerInput.h"

class Collider;
class PlayerInput;
class ZeroState;
class Player: public Pawn
{
private:
	Collider* PlayerCollider;
	 
	ZeroState* state;

	PlayerInput* playerInput;
	std::unordered_map<UINT, InputAction> keyActionMap;

	float speed;

	typedef std::function<void(Player&)> stateFunction;

	// FSM: input, function binding
	std::unordered_map<InputAction, stateFunction> inputStateMap;

public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
	void MakeSnapShot(void* out) override;

	void BindState();

	void Left();
	void Right();
	void Down();
	void Jump();
	void Fall();
	void Attack();
	void Dead();
	void WallSlide();
};

