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
	EnemyState* eState;				// �������Ͽ� Ŭ����, delete �߰�
	GPImage* image;					// GDI+ �̹���, delete �߰�
	vector<GPImage*> images;		// �� ĳ���Ϳ� �ʿ��� ��� �̹���, ����س��� �ҷ����� ��, delete �߰�
	string currAnimKey;
	float Speed;					// �̵��ӵ�
	int currFrame;					// �ִϸ��̼�
	float frameTimer;				// �ִϸ��̼� ������ƮŸ��
	bool bFlip;						// ������
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
