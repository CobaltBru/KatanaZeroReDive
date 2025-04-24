#include "GPImageManager.h"
void GPImageManager::Init()
{
    InitImages();
}

void GPImageManager::Release()
{
    // 등록된 이미지 모두 해제
    for (auto& kv : mapImages) {
        if (kv.second) {
            kv.second->Release();
            delete kv.second;
            kv.second = nullptr;
        }
    }
    mapImages.clear();
    // 싱글톤 인스턴스 해제
    ReleaseInstance();
}

void GPImageManager::InitImages()
{
    // 필요한 이펙트들 추가해놓기
    AddImage("bloodparticle", L"Image/fx/bloodparticle.png", 1, 1);
    AddImage("normalslash", L"Image/fx/NormalSlash.png", 5, 1);
    AddImage("rainbowslash", L"Image/fx/RainbowSlash.png", 7, 1);
    AddImage("bulletreflect", L"Image/fx/BulletReflect.png", 5, 1);
    AddImage("hitslash", L"Image/fx/HitSlash.png", 4, 1);
    AddImage("enemyslash", L"Image/fx/EnemySlash.png", 4, 1);
    AddImage("jumpcloud", L"Image/fx/JumpCloud.png", 4, 1);
    AddImage("dustcloud", L"Image/fx/spr_dustcloud.png", 7, 1);
    AddImage("landcloud", L"Image/fx/spr_landcloud.png", 7, 1);
    AddImage("BGBlood_right1", L"Image/fx/BGBlood_right1.png");
    AddImage("BGBlood_right2", L"Image/fx/BGBlood_right2.png");
    AddImage("BGBlood_right3", L"Image/fx/BGBlood_right3.png");
    AddImage("BGBlood_righttop1", L"Image/fx/BGBlood_righttop1.png");
    AddImage("BGBlood_righttop2", L"Image/fx/BGBlood_righttop2.png");
    AddImage("BGBlood_righttop3", L"Image/fx/BGBlood_righttop3.png");
    AddImage("BGBlood_rightbottom1", L"Image/fx/BGBlood_rightbottom1.png");
    AddImage("BGBlood_rightbottom2", L"Image/fx/BGBlood_rightbottom2.png");
    AddImage("BGBlood_rightbottom3", L"Image/fx/BGBlood_rightbottom3.png");
}

GPImage* GPImageManager::AddImage(const std::string& key,
    const wchar_t* filePath)
{
    auto it = mapImages.find(key);
    if (it != mapImages.end())
        return it->second;

    GPImage* img = new GPImage();
    img->AddImage(filePath);
    mapImages[key] = img;
    return img;
}

GPImage* GPImageManager::AddImage(const std::string& key,
    const wchar_t* filePath,
    int frameX,
    int frameY)
{
    auto it = mapImages.find(key);
    if (it != mapImages.end())
        return it->second;

    GPImage* img = new GPImage();
    img->AddImage(filePath, frameX, frameY);
    mapImages[key] = img;
    return img;
}

GPImage* GPImageManager::FindImage(const std::string& key) const
{
    auto it = mapImages.find(key);
    return (it != mapImages.end()) ? it->second : nullptr;
}

void GPImageManager::DeleteImage(const std::string& key)
{
    auto it = mapImages.find(key);
    if (it == mapImages.end())
        return;

    it->second->Release();
    delete it->second;
    mapImages.erase(it);
}