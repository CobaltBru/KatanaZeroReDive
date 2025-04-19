#pragma once
#include "Enemy.h"
#include "config.h"

class Grunt : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

