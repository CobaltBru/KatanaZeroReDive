#pragma once
#include "Scene.h"

class Image;
class LoadingScene : public Scene
{
private:
	Image* bg;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual ~LoadingScene() {};

protected:
	virtual HRESULT InitImage() { return S_OK; };
	virtual HRESULT InitObject() { return S_OK; };

};

