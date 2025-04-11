#pragma once
#include "GameObject.h"

class Stage1Scene : public GameObject
{
public:
	Stage1Scene() = default;
	virtual ~Stage1Scene() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

