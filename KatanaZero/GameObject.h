#pragma once
#include "config.h"

class GameObject
{
public:
	virtual HRESULT Init();		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);

	GameObject();
	virtual ~GameObject();
};

