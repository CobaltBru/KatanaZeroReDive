#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <stack>
#include "GameObject.h"





class Player : public GameObject
{
private:
	
	
	float scrollSpeed;

	
	// 리플레이용 키
	string currAnimKey;

	Image* image;
	Image* effectImage;

	Collider* AttackCollider;

public:
	Player();
	virtual ~Player();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void InitImage();
	
	// rigid body
	void InitRigidBody();
	void UpdateRigidBody();

	// scroll
	void InitScrollOffset();
	void Offset();

	inline Image* GetImage() { return image; }
	inline void SetImage(Image* image) { this->image = image; }

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