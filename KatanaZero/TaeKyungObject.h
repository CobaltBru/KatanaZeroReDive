#pragma once
#include "GameObject.h"

class Image;
class Collider;
class PlayerSnapShot;
class TaeKyungObject : public GameObject
{
public:
	TaeKyungObject();
	virtual ~TaeKyungObject() = default;

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const PlayerSnapShot& snapShot);

private:
	void Move();
	void Collision();

	void InitOffset();
	void Offset();
private:
	Image* Image;
	Collider* ObjectCollider;
	float Speed;
};

