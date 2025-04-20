#pragma once
#include "Enemy.h"
#include "config.h"

class Grunt : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;

	// Enemy별 행동 따로 구현
	virtual void Patrol() override;
	virtual void Attack() override;
	virtual void Chasing() override;
};

