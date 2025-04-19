#pragma once
#include "GameObject.h"
#include "config.h"
#include "EnemyState.h"

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
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);
	inline bool IsDead() { return bDead; }
	EnemyState* GetState() const { return eState; }

	void UpdateAnimation();
	void ChangeState(EnemyState* newState);
	void ChangeAnimation(EImageType newImage);
	virtual bool Detecting();
};

