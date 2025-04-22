#include "GPImageManager.h"
void GPImageManager::Init()
{
}

void GPImageManager::Release()
{

    // ��ϵ� �̹��� ��� ����
    for (auto& kv : mapImages) {
        if (kv.second) {
            kv.second->Release();
            delete kv.second;
            kv.second = nullptr;
        }
    }
    mapImages.clear();

    // �̱��� �ν��Ͻ� ����
    ReleaseInstance();
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