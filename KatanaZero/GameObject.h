#pragma once
#include "config.h"


class Collider;
class RigidBody;
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual HRESULT Init();
	virtual HRESULT Init(FPOINT Pos);
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);

	FPOINT GetPos() const { return Pos; }
	FPOINT GetSpawnPos() { return spawnPos; }
	bool IsDead() const { return bDead; }

	void SetPos(FPOINT InPos) { Pos = InPos; }
	void SetSpawnPos(FPOINT spawnPos) { this->spawnPos = spawnPos; }
	void SetDead(bool InDead) { bDead = InDead; }

	int GetFloorIndex() const;

protected:
	FPOINT Pos;

	FPOINT spawnPos;
	bool bDead;

	

	//�׽�Ʈ�ڵ� �°�
public:
	RigidBody* GetRigidBody() const { return ObjectRigidBody; }
	Collider* GetCollider() const { return ObjectCollider; }
	FPOINT GetLastPos() const { return LastPos; }
protected:
	Collider* ObjectCollider;
	RigidBody* ObjectRigidBody;
	FPOINT LastPos;

};

