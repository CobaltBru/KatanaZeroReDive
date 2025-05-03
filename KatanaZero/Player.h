#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <stack>
#include "GameObject.h"
#include "INode.h"

enum STATE { IDLE, WALK, LOW, AIR, WALL, ATTACK, ROLL, DEAD, END };

class Image;
class Collider;
class Action;
class Animator;
class UIGame;
class ArrowUI;
class PickUpHand;
class Player : public GameObject
{
private:


	float scrollSpeed;
	int way;
	bool isEffect;
	// 리플레이용 키
	string currAnimKey;

	Image* image;
	Image* effectImage;

	Animator* animator;

	STATE currentState;
	STATE oldState;
	vector<Action*> stateMachine;

	float ScrollSpeed;
	FPOINT scroll;

	PickUpHand* RightHand;
	UIGame* UIGameObj;
	ArrowUI* ArrowUIObj;

	bool canUseSkill1;

public:
	Player();
	virtual ~Player();

	HRESULT Init() override;
	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize,
		bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend)override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void InitImage();
	void InitAnimator();
	// rigid body
	void InitRigidBody();
	void UpdateRigidBody();

	// scroll
	void InitScrollOffset();
	void Offset();

	void StateInit();
	void changeState(STATE state);
	inline STATE getOldState() { return oldState; }
	inline int* GetWay() { return &way; }
	inline Animator* GetAnimator() { return animator; }
	string stateToString();

	void SetUI(UIGame* InUIGame) { UIGameObj = InUIGame; }

	void SetArrowUI(ArrowUI* InArrowUI) { ArrowUIObj = InArrowUI; }
	void PickUpUpdate();
	void Shoot();
};
