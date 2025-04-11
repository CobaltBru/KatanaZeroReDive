#pragma once
#include "GameObject.h"

class Image;
class TaeKyungObject : public GameObject
{
public:
	TaeKyungObject() = default;
	virtual ~TaeKyungObject() = default;

	virtual HRESULT Init();		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);	

private:
	Image* Image;
};

