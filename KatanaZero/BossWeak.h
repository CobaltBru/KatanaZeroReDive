#pragma once
#include "GameObject.h"

class Image;
class BossWeak : public GameObject
{
public:
	BossWeak();
	virtual ~BossWeak();

	HRESULT Init(FPOINT InPos,FPOINT TargetPos);

	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	bool IsHit() const { return bHit; }
private:
	Image* Image;
	FPOINT TargetPos;
	bool bHit;
	float frameX;
};

