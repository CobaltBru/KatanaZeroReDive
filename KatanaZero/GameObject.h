#pragma once
#include "config.h"
#include "RenderManager.h"

class Collider;
class RigidBody;
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual HRESULT Init();
	virtual HRESULT Init(FPOINT Pos);
	virtual	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);

	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);

	FPOINT GetPos() const { return Pos; }
	FPOINT* GetPPos() { return &Pos; }
	FPOINT GetSpawnPos() { return spawnPos; }
	virtual bool IsDead() const { return bDead; }
	bool GetFlip() const { return bFlip; }
	float GetScale() const { return Scale; }
	//bool IsDead() const { return bDead; }

	void SetPos(FPOINT InPos) { Pos = InPos; }
	void SetSpawnPos(FPOINT spawnPos) { this->spawnPos = spawnPos; }
	void SetDead(bool InDead) { bDead = InDead; }
	void SetScale(float InScale) { Scale = InScale; }
	void SetFlip(bool InFlip) { bFlip = InFlip; }

	int GetFloorIndex(vector<FloorZone> fz) const;

protected:
	FPOINT Pos;

	FPOINT spawnPos;
	bool bDead;

	//테스트코드 태경
public:
	RigidBody* GetRigidBody() const { return ObjectRigidBody; }
	Collider* GetCollider() const { return ObjectCollider; }
	FPOINT GetLastPos() const { return LastPos; }
protected:
	Collider* ObjectCollider;
	RigidBody* ObjectRigidBody;
	FPOINT LastPos;
	ERenderGroup RenderGroup;
	bool bFlip;
	float Scale;

};

