#include "EffectManager.h"
#include "Effect.h"

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

void EffectManager::Addfx(string key, const wchar_t* filePath)
{
    Effect* fx = nullptr;
    fx = Findfx(key);
    if (fx) return;
    fx = new Effect();
    if (FAILED(fx->Init(filePath)))
    {
        fx->Release();
        delete fx;
        return;
    }
    mapFx.insert(make_pair(key, fx));
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
