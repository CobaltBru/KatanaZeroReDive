#pragma once
#include "Singleton.h"
#include "GPImage.h"
#include <map>
#include <string>

// GPImage ���� ���� ���ҽ� �Ŵ���
class GPImageManager : public Singleton<GPImageManager>
{
public:
    // GDI+ �� �ʱ�ȭ
    void Init();
    // ��� GPImage�� �� ����
    void Release();

    // �̹��� �ε� (���� ������)
    GPImage* AddImage(const std::string& key,
        const wchar_t* filePath);
    // ��������Ʈ ��Ʈ �ε� (���� ������)
    GPImage* AddImage(const std::string& key,
        const wchar_t* filePath,
        int frameX,
        int frameY);

    // �˻� �� ����
    GPImage* FindImage(const std::string& key) const;
    void DeleteImage(const std::string& key);

private:
    std::map<std::string, GPImage*> mapImages;
};

