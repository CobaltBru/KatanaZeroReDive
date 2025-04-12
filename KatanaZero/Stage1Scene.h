#pragma once
#include "Scene.h"

class Stage1Scene : public Scene
{
public:
	Stage1Scene() = default;
	virtual ~Stage1Scene() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual HRESULT ImageInit();
	virtual HRESULT ObjectInit();
};

