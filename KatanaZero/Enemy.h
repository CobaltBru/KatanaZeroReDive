#pragma once
#include "GameObject.h"
#include "config.h"
#include "EnemyState.h"

class GPImage;
class Collider;
class RigidBody;
class Enemy : public GameObject
{
protected:
	EnemyState* eState;				// �������Ͽ� Ŭ����
	GPImage* image;					// GDI+ �̹���
	vector<GPImage*> images;		// �� ĳ���Ϳ� �ʿ��� ��� �̹���, ����س��� �ҷ����� ��
	Collider* ObjectCollider;		// �浹 üũ��
	RigidBody* ObjectRigidBody;		// ���� ����
	float Speed;					// �̵��ӵ�
	int currFrame;					// �ִϸ��̼�
	float frameTimer;				// �ִϸ��̼� ������ƮŸ��
	bool bFlip;						// ������
	
	// ������ٵ𿡼� ���µ�?
	FPOINT LastPos;
public:
	Enemy();
	virtual ~Enemy();
	virtual HRESULT Init(FPOINT InPos);
	virtual void InitImages();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	inline bool IsDead() { return bDead; }
	EnemyState* GetState() const { return eState; }
	Collider* GetCollider() const { return ObjectCollider; }
	FPOINT GetLastPos()  const { return LastPos; }

	void UpdateAnimation();
	void ChangeState(EnemyState* newState);
	void ChangeAnimation(EImageType newImage);
	bool Detecting();
};

