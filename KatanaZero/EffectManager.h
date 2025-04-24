#pragma once
#include "Singleton.h"
#include "config.h"
#include <map>
#include <random>
#include "Effect.h"

class GPImage;
class Image;
class EffectManager : public Singleton<EffectManager>
{
private:
	map<string, Effect*> mapFx;
	vector<Effect*> activeFx;
	vector<RemainEffect> remainFx;
	vector<BackgroundBloodfx> bgBloodFx;
	mt19937 rng{ random_device{}() };
	
public:
	void Init();
	void InitEffects();
	void Release();
	void Update();
	void Render(HDC hdc);
	void Addfx(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY);
	void Addfx(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY, FPOINT start, FPOINT end, float speed, bool bMove = false);
	void Addfx(string key, FPOINT start, FPOINT end, float speed, bool bMove = false);
	Effect* Findfx(string key);
	void Activefx(string key, FPOINT pos, float angle, bool bFlip);
	void Activefx(string key, FPOINT pos, FPOINT dest, float speed, bool bFlip);
	void CreateRemainEffect(GPImage* image, FPOINT pos, int frame, bool bFlip = false);
	void CreateBGBlood(FPOINT pos, float angle, FPOINT size);
	void EmitBlood(FPOINT pos, int count);
};

