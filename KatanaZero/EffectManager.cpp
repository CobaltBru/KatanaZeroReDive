#include "EffectManager.h"
#include "SnapShot.h"
#include "SnapShotManager.h"
#include "GPImage.h"
#include "Image.h"
#include <algorithm>

void EffectManager::Init()
{
    Addfx("gangstergun", L"Image/fx/Gangster_FireFx.png", 6, 1);
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

    for (auto& iter : activeFx)
    {
        delete iter;
    }
    activeFx.clear();

    ReleaseInstance();
}

void EffectManager::Update()
{
    for (auto& fx : activeFx)
    {
        fx->Update();
    }
    activeFx.erase(remove_if(activeFx.begin(), activeFx.end(), [](Effect* e) {
        if (!e->IsActive()) 
        { 
            delete e; 
            return true; 
        }
        return false; }), activeFx.end());

    float dt = TimerManager::GetInstance()->GetDeltaTime();
    for (auto& riter : remainFx)
    {
        riter.lifetime -= dt;
        riter.alpha = riter.lifetime / 0.5f;
    }
    remainFx.erase(remove_if(remainFx.begin(), remainFx.end(), [](RemainEffect& e) { 
        if (e.lifetime <= 0) { 
            return true; 
        }
        return false; 
    }), remainFx.end());
}

void EffectManager::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    for (auto& rIter : remainFx)
    {
        rIter.image->Middle_RenderFrame(&graphics, rIter.pos, rIter.frame, rIter.bFlip, rIter.alpha);
    }
    
    for (auto& fx : activeFx)
    {
        fx->Render(hdc);
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

void EffectManager::Addfx(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY, FPOINT start, FPOINT end, float speed, bool bMove)
{
    Effect* fx = nullptr;
    fx = Findfx(key);
    if (fx) return;
    fx = new Effect();
    if (FAILED(fx->Init(filePath, maxFrameX, maxFrameY, start, end, speed, bMove)))
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
            //SnapShotManager::GetInstance()->AddGameObject(EObjectClassType::Effect, iter->second);
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
    Effect* newfx = new Effect(*fx); // ¾èº¹ -> ´ó±Û¸µ »µÅ· Æ÷ÀÎÅÍ ¹ß»ý   << ±â¸ð¸µ~
    newfx->Activefx(pos, angle, bFlip);
    activeFx.push_back(newfx);
    //SnapShotManager::GetInstance()->AddGameObject(EObjectClassType::Effect, newfx);
}

void EffectManager::Activefx(string key, FPOINT pos, FPOINT dest, float speed, bool bFlip)
{
    Effect* fx = Findfx(key);
    if (!fx) return;
    Effect* newfx = new Effect(*fx);
    newfx->Activefx(pos, dest, speed, bFlip);
    activeFx.push_back(newfx);
    //SnapShotManager::GetInstance()->AddGameObject(EObjectClassType::Effect, newfx);
}

void EffectManager::CreateRemainEffect(GPImage* image, FPOINT pos, int frame, bool bFlip)
{
    RemainEffect rFx;
    rFx.image = image;
    rFx.pos = pos;
    rFx.frame = frame;
    rFx.bFlip = bFlip;
    rFx.lifetime = 0.2f;
    rFx.alpha = 1.0f;

    remainFx.push_back(rFx);
}
