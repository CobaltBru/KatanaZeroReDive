#pragma once
#include "GameObject.h"
#include "RenderManager.h"
#include "Image.h"

class DefaultObject : public GameObject
{
public:
	DefaultObject();
	virtual ~DefaultObject() = default;

	HRESULT Init(string InImageKey,FPOINT InPos, bool InFlip, ERenderGroup InRenderGroup, string InClassName);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	string GetName() const { return ClassName; }
	string GetImageName() const { return ImageName; }
	bool GetFlip() const { return bFlip; }
	POINT GetImageSize() const { return { Image->GetFrameWidth(),Image->GetFrameHeight() }; }

	void SetFlip(bool InFlip) { bFlip = InFlip; }
private:
	Image* Image;
	bool bFlip;
	ERenderGroup RenderGroup;
	string ClassName;
	string ImageName;
};

