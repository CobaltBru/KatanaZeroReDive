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
	EnemyState* eState;				// 상태패턴용 클래스
	GPImage* image;					// GDI+ 이미지
	vector<GPImage*> images;		// 한 캐릭터에 필요한 모든 이미지, 등록해놓고 불러오기 용
	Collider* ObjectCollider;		// 충돌 체크용
	RigidBody* ObjectRigidBody;		// 물리 적용
	float Speed;					// 이동속도
	int currFrame;					// 애니메이션
	float frameTimer;				// 애니메이션 업데이트타임
	bool bFlip;						// 뒤집기
	
	// 리지드바디에서 쓰는듯?
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

