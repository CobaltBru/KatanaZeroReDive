#pragma once
#include "GameObject.h"

class Image;
class NormalParts : public GameObject
{
public:
	NormalParts();
	~NormalParts() = default;

	HRESULT Init(string InImageKey,FPOINT InPos, ERenderGroup InRenderGroup);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	int GetFrameX() const { return FrameX; }
	int GetMaxFrameX();
	string GetImageKey() const { return CurrentImageKey; }

	void ChangeImage(string InImageKey, bool InLastFrameStop = false);

private:
	Image* Image;
	string CurrentImageKey;
	float CurrentTime;
	int FrameX;
	bool bLastFrameStop;
};

