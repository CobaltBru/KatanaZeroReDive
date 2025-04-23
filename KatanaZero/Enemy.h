#pragma once
#include "GameObject.h"
#include "config.h"
#include "EnemyState.h"

enum class EType
{
	Grunt,
	Pomp,
	Gangster,
	ShieldCop,
	None
};

class GPImage;
class Enemy : public GameObject
{
protected:
	EnemyState* eState;				// 상태패턴용 클래스, delete 했고
	GPImage* image;					// GDI+ 이미지, delete 했고
	vector<GPImage*> images;		// 한 캐릭터에 필요한 모든 이미지, 등록해놓고 불러오기 용, delete 했고
	string currAnimKey;
	float Speed;					// 이동속도
	int currFrame;					// 애니메이션
	float frameTimer;				// 애니메이션 업데이트타임
	bool bFlip;						// 뒤집기
	int dir;
	float detectRange;
	float attackRange;
	float meleeAttackRange;
	float attackDuration;
	EType eType;
	int targetFloor;
	bool bReachedTargetFloor;

	// Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };
	
public:
	Enemy();
	virtual ~Enemy();
	virtual HRESULT Init(FPOINT InPos);
	virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup);
	virtual void InitImages();
	virtual void InitRigidBodySetting();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);
	EnemyState* GetState() const { return eState; }
	int GetMaxAttackFrame() const;
	int GetCurrFrame() const { return currFrame; }
	EType GetEnemyType() const { return eType; }
	int GetDir() const { return dir; }
	void SetDir(int InDir) { dir = InDir; }
	float GetSpeed() const { return Speed; }
	GPImage* GetImage() const { return image; }
	void SetTargetFloor(int floor) { targetFloor = floor; }
	int GetTargetFloor() const { return targetFloor; }
	void SetReachedTargetFloor(bool value) { bReachedTargetFloor = value; }
	bool HasReachedTargetFloor() const { return bReachedTargetFloor; }

	void UpdateAnimation();
	void ChangeState(EnemyState* newState);
	void ChangeAnimation(EImageType newImage);
	virtual void SetAnimKey(EImageType newImage) {};
	virtual bool Detecting();
	virtual bool IsInAttackRange();
	virtual bool IsInMeleeAttackRange();
	virtual bool IsInSameFloor();
	virtual bool IsOnDownLine();
	bool IsHitted();

	virtual void Attack() {};
};
