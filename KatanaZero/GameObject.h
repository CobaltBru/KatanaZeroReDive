#pragma once
#include "config.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual HRESULT Init();		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);

	FPOINT GetPos() const { return Pos; }

protected:
	FPOINT Pos;
};

