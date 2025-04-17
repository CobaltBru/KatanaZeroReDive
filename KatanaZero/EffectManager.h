#pragma once
#include "Singleton.h"
#include "config.h"
#include <map>

class Effect;
class EffectManager : public Singleton<EffectManager>
{
private:
	map<string, Effect*> mapFx;
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	void Addfx(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY);
	void RegisterEffect();
	Effect* Findfx(string key);
	void Activefx(string key, FPOINT pos, float angle, bool bFlip);
};

