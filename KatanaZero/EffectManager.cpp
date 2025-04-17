#include "EffectManager.h"
#include "Effect.h"
#include "SnapShot.h"
#include "SnapShotManager.h"

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
}

void EffectManager::Render(HDC hdc)
{
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
