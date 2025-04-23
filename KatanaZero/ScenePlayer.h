#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"

class ScenePlayer : public GameObject
{
private:
	Collider* ObjectCollider;
	RigidBody* ObjectRigidBody;
	float Speed;

	Animation ani[7];
	int aniIdx;
public:
	ScenePlayer();
	virtual ~ScenePlayer() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

