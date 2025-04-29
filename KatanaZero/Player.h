#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <stack>
#include "GameObject.h"
#include "INode.h"

class Image;
class Collider;
class Player : public GameObject
{
private:
	
	
	float scrollSpeed;

	
	// 리플레이용 키
	string currAnimKey;

	Image* image;
	Image* effectImage;

	Collider* ObjectCollider;

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

	//ground
	NodeStatus isGround();
	NodeStatus isInput();
	NodeStatus isRoll();
	NodeStatus isKeyDown();
	NodeStatus isKeyLeftRight();

	NodeStatus actionIdle();
	NodeStatus actionRoll();
	NodeStatus actionDown();
	NodeStatus actionMove();



};
