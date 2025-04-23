#pragma once
#include "Singleton.h"
#include "config.h"

///*
//	�迭 : �ε��� -> ������
//	stl::vector : �ε��� -> ������
//	std::map : Ű -> ������
//
//	����-�� Ʈ�� ���� : ����, ����, �˻� O(logN) �ð����⵵
//*/

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mapImages;

public:
	void Init();
	void Release();

	Image* AddImage(string key, const wchar_t* filePath,
		int width, int height,
		bool isTransparent = FALSE,
		COLORREF transColor = FALSE);

	Image* AddImage(string key, const wchar_t* filePath, 
		int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTransparent = FALSE, 
		COLORREF transColor = FALSE);

	Image* AddImage(string key, const wchar_t* filePath,
		bool isTransparent = FALSE,
		COLORREF transColor = FALSE, int FrameWidth = 0,int FramwHeight = 0);

	void DeleteImage(string key);
	Image* FindImage(string key);
};

