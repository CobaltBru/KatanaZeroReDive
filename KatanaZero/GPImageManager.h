#pragma once
#include "Singleton.h"
#include "GPImage.h"
#include <map>
#include <string>

// GPImage 전용 공유 리소스 매니저
class GPImageManager : public Singleton<GPImageManager>
{
public:
    // GDI+ 펜 초기화
    void Init();
    // 모든 GPImage와 펜 해제
    void Release();

    // 이미지 로드 (단일 프레임)
    GPImage* AddImage(const std::string& key,
        const wchar_t* filePath);
    // 스프라이트 시트 로드 (다중 프레임)
    GPImage* AddImage(const std::string& key,
        const wchar_t* filePath,
        int frameX,
        int frameY);

    // 검색 및 삭제
    GPImage* FindImage(const std::string& key) const;
    void DeleteImage(const std::string& key);

private:
    std::map<std::string, GPImage*> mapImages;
};

