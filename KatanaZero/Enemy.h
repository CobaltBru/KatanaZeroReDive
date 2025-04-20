#pragma once
#include "GameObject.h"
#include "config.h"
#include "EnemyState.h"

enum class EType
{
	Grunt,
	Pomp,
	Gangster,
	SheildCop,
	None
};

class GPImage;
class Enemy : public GameObject
{
protected:
	EnemyState* eState;				// �������Ͽ� Ŭ����
	GPImage* image;					// GDI+ �̹���
	vector<GPImage*> images;		// �� ĳ���Ϳ� �ʿ��� ��� �̹���, ����س��� �ҷ����� ��
	float Speed;					// �̵��ӵ�
	int currFrame;					// �ִϸ��̼�
	float frameTimer;				// �ִϸ��̼� ������ƮŸ��
	bool bFlip;						// ������
	int dir;
	float detectRange;
	float attackRange;
	EType eType;

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
	virtual void InitImages();
	virtual void InitRigidBodySetting();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);
	inline bool IsDead() { return bDead; }
	EnemyState* GetState() const { return eState; }
	int GetMaxAttackFrame() const;
	int GetCurrFrame() const { return currFrame; }
	EType GetEnemyType() const { return eType; }
	int GetDir() const { return dir; }
	void SetDir(int InDir) { dir = InDir; }
	float GetSpeed() const { return Speed; }

	void UpdateAnimation();
	void ChangeState(EnemyState* newState);
	void ChangeAnimation(EImageType newImage);
	virtual bool Detecting();
	virtual bool IsInAttackRange();

	// �ൿ
	virtual void Patrol() {};
	virtual void Attack() {};
	virtual void Chasing() {};
};
