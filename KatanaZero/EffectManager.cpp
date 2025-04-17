#include "EffectManager.h"
#include "SnapShot.h"
#include "SnapShotManager.h"
#include "GPImage.h"
#include <algorithm>

void EffectManager::Init()
{
}

void EffectManager::Release()
{
    map<string, Effect*>::iterator iter;
    for (iter = mapFx.begin(); iter != mapFx.end(); iter++)
    {
        if (iter->second)
        {
            iter->second->Release();
            delete iter->second;
            iter->second = nullptr;
        }
    }
    mapFx.clear();
    ReleaseInstance();
}

void EffectManager::Update()
{
    map<string, Effect*>::iterator iter;
    for (iter = mapFx.begin(); iter != mapFx.end(); iter++)
    {
        if (iter->second)
            iter->second->Update();
    }

    float dt = TimerManager::GetInstance()->GetDeltaTime();
    for (auto& riter : remainFx)
    {
        riter.lifetime -= dt;
        riter.alpha = riter.lifetime / 0.5f;
    }
    remainFx.erase(remove_if(remainFx.begin(), remainFx.end(), [](RemainEffect& e) { return e.lifetime <= 0; }), remainFx.end());
}

void EffectManager::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    for (auto& rIter : remainFx)
    {
        rIter.image->Middle_RenderFrame(&graphics, rIter.pos, rIter.frame, rIter.bFlip, rIter.alpha);
    }
    map<string, Effect*>::iterator iter;
    for (iter = mapFx.begin(); iter != mapFx.end(); iter++)
    {
        if (iter->second)
            iter->second->Render(hdc);
    }
}

void EffectManager::Addfx(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY)
{
    Effect* fx = nullptr;
    fx = Findfx(key);
    if (fx) return;
    fx = new Effect();
    if (FAILED(fx->Init(filePath, maxFrameX, maxFrameY)))
    {
        fx->Release();
        delete fx;
        return;
    }
    mapFx.insert(make_pair(key, fx));
}

void EffectManager::RegisterEffect()
{
    map<string, Effect*>::iterator iter;
    for (iter = mapFx.begin(); iter != mapFx.end(); iter++)
    {
        if (iter->second)
        {
            SnapShotManager::GetInstance()->AddGameObject(EObjectClassType::Effect, iter->second);
        }
    }
}

Effect* EffectManager::Findfx(string key)
{
    map<string, Effect*>::iterator iter;
    iter = mapFx.find(key);

    if (iter == mapFx.end()) return nullptr;
    return iter->second;
}

void EffectManager::Activefx(string key, FPOINT pos, float angle, bool bFlip)
{
    Effect* fx = Findfx(key);
    if (!fx) return;
    fx->Activefx(pos, angle, bFlip);
}

void EffectManager::CreateRemainEffect(GPImage* image, FPOINT pos, int frame, bool bFlip)
{
    RemainEffect rFx;
    rFx.image = image;
    rFx.pos = pos;
    rFx.frame = frame;
    rFx.bFlip = bFlip;
    rFx.lifetime = 0.3f;
    rFx.alpha = 1.0f;

    remainFx.push_back(rFx);
}
