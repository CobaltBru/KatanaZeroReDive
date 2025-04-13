#pragma once
#include "GameObject.h"

class Scene : public GameObject
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual HRESULT Init() { return S_OK; };
	virtual void Release() {};
	virtual void Update() {};
	virtual void Render(HDC hdc) {};

protected:
	virtual HRESULT InitImage() = 0;
	virtual HRESULT InitObject() = 0;
};

