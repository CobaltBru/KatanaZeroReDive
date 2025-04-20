#pragma once
#include "Enemy.h"
#include "config.h"

class Grunt : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;

	// Enemy�� �ൿ ���� ����
	virtual void Patrol() override;
	virtual void Attack() override;
	virtual void Chasing() override;
};

