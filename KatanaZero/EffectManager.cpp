#include "EffectManager.h"
#include "SnapShot.h"
#include "SnapShotManager.h"
#include "GPImage.h"
#include "GPImageManager.h"
#include "Image.h"
#include <algorithm>
#include "ScrollManager.h"

void EffectManager::Init()
{
    InitEffects();
}

void EffectManager::InitEffects()
{
    // 디폴트 프레임 업데이트 시간 0.1초
    Addfx("gangstergun", {}, {}, 0.f);
    Addfx("normalslash", {}, {}, 0.f);
    Addfx("rainbowslash", {}, {}, 0.f);
    Addfx("bulletreflect", {}, {}, 0.f);
    Addfx("hitslash", {}, {}, 0.f);
    Addfx("enemyslash", {}, {}, 0.f);
    Addfx("jumpcloud", {}, {}, 0.f);
    Addfx("wallcloud", {}, {}, 0.f);
    Addfx("landcloud", {}, {}, 0.f);
    Addfx("dustcloud", {}, {}, 0.f, 0.2f);
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
    remainFx.clear();
    bgBloodFx.clear();

    ReleaseInstance();
}

void EffectManager::Update()
{
    for (auto& bg : bgBloodFx)
    {
        bg.Update();
    }
    for (auto fiter = activeFx.begin(); fiter != activeFx.end();)
    {
        if ((*fiter)->IsActive())
        {
            (*fiter)->Update();
            ++fiter;
        }
        else
        {
            (*fiter)->Release();
            delete (*fiter);
            fiter = activeFx.erase(fiter);
            continue;
        }
    }

    float dt = TimerManager::GetInstance()->GetDeltaTime();
    for (auto& riter : remainFx)
    {
        riter.lifetime -= dt;
        if (riter.lifetime > 0.f)
        {
            float gravity = 800.f;
            float drag = 0.2f;
            if (riter.velocity.x != 0 || riter.velocity.y != 0)
            {
                riter.pos.x += riter.velocity.x * dt;
                riter.velocity.y += gravity * dt;
                riter.velocity *= (1.f - drag * dt);
                riter.pos.y += riter.velocity.y * dt;
            }
            
        }
        float t = riter.totalLife - riter.lifetime;
        float f = t / riter.totalLife;
        float n = 3.f;
        riter.alpha = 1.f - pow(f, n);
        //riter.alpha = riter.lifetime / riter.totalLife;
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
        rIter.image->Middle_RenderFrameScale(&graphics, rIter.pos, rIter.frame, rIter.bFlip, rIter.alpha, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
    }
    
    for (auto& fx : activeFx)
    {
        fx->Render(hdc);
    }
}

void EffectManager::Addfx(string key, FPOINT start, FPOINT end, float speed, float duration, bool bMove)
{
    Effect* fx = nullptr;
    fx = Findfx(key);
    if (fx) return;
    fx = new Effect();
    if (FAILED(fx->Init(key, start, end, speed, duration, bMove)))
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
    Effect* newfx = new Effect(*fx); // 얕복 -> 댕글링 뻐킹 포인터 발생   << 기모링~
    newfx->Activefx(pos, angle, bFlip);
    activeFx.push_back(newfx);
}

void EffectManager::Activefx(string key, FPOINT pos, float angle, float speed, bool bFlip)
{
    Effect* fx = Findfx(key);
    if (!fx) return;
    Effect* newfx = new Effect(*fx); // 얕복 -> 댕글링 뻐킹 포인터 발생   << 기모링~
    newfx->Activefx(pos, angle, bFlip);
    activeFx.push_back(newfx);
}

void EffectManager::Activefx(string key, FPOINT pos, FPOINT dest, float speed, bool bFlip)
{
    Effect* fx = Findfx(key);
    if (!fx) return;
    Effect* newfx = new Effect(*fx);
    newfx->Activefx(pos, dest, speed, bFlip);
    activeFx.push_back(newfx);
}

void EffectManager::Activefx(string key, FPOINT pos, float angle, GameObject* owner, bool bFlip)
{
    Effect* fx = Findfx(key);
    if (!fx) return;
    Effect* newfx = new Effect(*fx);
    newfx->Activefx(pos, angle, owner, bFlip);
    activeFx.push_back(newfx);
}

void EffectManager::CreateRemainEffect(GPImage* image, FPOINT pos, int frame, bool bFlip)
{
    RemainEffect rFx;
    rFx.image = image;
    rFx.pos = pos;
    rFx.frame = frame;
    rFx.bFlip = bFlip;
    rFx.totalLife = rFx.lifetime = 0.2f;
    rFx.alpha = 1.0f;

    remainFx.push_back(rFx);
}

void EffectManager::CreateBGBlood(FPOINT pos, float angle, FPOINT size)
{
    FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
    int rndimg = rand() % 3;
    BackgroundBloodfx bgBlood;
    bgBlood.image = nullptr;
    if (angle > -30.f && angle <= 30.f)
    {
        switch (rndimg)
        {
        case 0:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_right1");
            break;
        case 1:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_right2");
            break;
        case 2:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_right3");
            break;
        }
        bgBlood.bFlip = true;
        float offset = (float)bgBlood.image->GetHeight() * ScrollManager::GetInstance()->GetScale() / 2.f;
        bgBlood.SetPos({ pos.x , pos.y - offset  });
    }
    else if (angle >= 30.f && angle < 90.f)
    {
        switch (rndimg)
        {
        case 0:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_righttop1");
            break;
        case 1:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_righttop2");
            break;
        case 2:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_righttop3");
            break;
        }
        bgBlood.bFlip = false;
        float offset = (float)bgBlood.image->GetHeight() * ScrollManager::GetInstance()->GetScale();
        bgBlood.SetPos({ pos.x , pos.y  - offset  });
    }
    else if (angle >= 90.f && angle < 150.f)
    {
        switch (rndimg)
        {
        case 0:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_righttop1");
            break;
        case 1:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_righttop2");
            break;
        case 2:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_righttop3");
            break;
        }
        bgBlood.bFlip = true;
        float offsetX = bgBlood.image->GetWidth() * ScrollManager::GetInstance()->GetScale();
        float offsetY = bgBlood.image->GetHeight() * ScrollManager::GetInstance()->GetScale();
        bgBlood.SetPos({ pos.x - offsetX , pos.y - offsetY });
    }
    else if ((angle >= 150.f && angle <= 180.f) || (angle >= -180.f && angle < -150.f))
    {
        switch (rndimg)
        {
        case 0:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_right1");
            break;
        case 1:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_right2");
            break;
        case 2:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_right3");
            break;
        }
        bgBlood.bFlip = false;
        float offsetX = bgBlood.image->GetWidth() * ScrollManager::GetInstance()->GetScale();
        float offsetY = bgBlood.image->GetHeight() * ScrollManager::GetInstance()->GetScale() / 2.f;
        bgBlood.SetPos({ pos.x - offsetX, pos.y - offsetY });
    }
    else if (angle >= -150.f && angle < -90.f)
    {
        switch (rndimg)
        {
        case 0:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_rightbottom1");
            break;
        case 1:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_rightbottom2");
            break;
        case 2:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_rightbottom3");
            break;
        }
        bgBlood.bFlip = true;
        float offset = bgBlood.image->GetWidth() * ScrollManager::GetInstance()->GetScale();
        bgBlood.SetPos({ pos.x - offset , pos.y });
    }
    else if (angle >= -90.f && angle < -30.f)
    {
        switch (rndimg)
        {
        case 0:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_rightbottom1");
            break;
        case 1:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_rightbottom2");
            break;
        case 2:
            bgBlood.image = ImageManager::GetInstance()->FindImage("BGBlood_rightbottom3");
            break;
        }
        bgBlood.bFlip = false;
        bgBlood.SetPos({ pos.x , pos.y });
    }
    bgBloodFx.push_back(bgBlood);
}

void EffectManager::EmitBlood(FPOINT pos, int count)
{
    
    for (int i = 0; i < count; i++)
    {
        uniform_real_distribution<float> angDist( 0.f, 2.f * 3.14159f );
        uniform_real_distribution<float> speedDist(300.f, 500.f);
        float angle = angDist(rng);
        float speed = speedDist(rng);
        RemainEffect p;
        p.image = GPImageManager::GetInstance()->FindImage("bloodparticle");
        p.pos.x = pos.x + ScrollManager::GetInstance()->GetScroll().x;
        p.pos.y = pos.y + ScrollManager::GetInstance()->GetScroll().y;
        p.totalLife = p.lifetime = 1.f;
        p.velocity = { cos(angle) * speed, sin(angle) * speed };
        p.alpha = 1.f;
        p.frame = 0;
        remainFx.push_back(p);
    }
}
