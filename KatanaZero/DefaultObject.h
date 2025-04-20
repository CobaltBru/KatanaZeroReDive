#pragma once
#include "GameObject.h"
#include "RenderManager.h"

class Image;
class DefaultObject : public GameObject
{
public:
	DefaultObject();
	virtual ~DefaultObject() = default;

	HRESULT Init(string InImageKey,FPOINT Pos, bool InFlip, ERenderGroup InRenderGroup);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	Image* Image;
	bool bFlip;
	ERenderGroup RenderGroup;
};

