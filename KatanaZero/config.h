// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")

#include <Windows.h>
#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include <random>
#include <GdiPlus.h>
#pragma comment(lib,"gdiplus")

using namespace std;
//using namespace Gdiplus;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif

#endif // _DEBUG

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"

/*
	�����Ϸ����� �ش� �ڵ带 �ڿ� ���ǵ� �ڵ�� �����Ѵ�. 
*/
#define WINSIZE_X	1080
#define WINSIZE_Y	500
#define TILEMAPTOOL_X	1420
#define TILEMAPTOOL_Y	700

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

typedef struct tagFPOINT
{
	float x;
	float y;
} FPOINT;

typedef struct tagLINE
{
	FPOINT LeftPoint;
	FPOINT RightPoint;

	tagLINE() {
		ZeroMemory(this, sizeof(tagLINE));
	}
	tagLINE(const FPOINT& InLeftPoint,const FPOINT& InRightPoint) {
		LeftPoint.x = InLeftPoint.x;
		LeftPoint.y = InLeftPoint.y;

		RightPoint.x = InRightPoint.x;
		RightPoint.y = InRightPoint.y;
	}
} LINE;

enum class ELineType
{
	Normal,
	DownLine,
	Wall,	
	Ceiling,
	End
};

/*
	extern Ű���� : ������ �Լ��� �ٸ� ���Ͽ� ���ǵǾ� �ִ� ���
	����� �˸��� Ű����.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

// random
extern random_device rd;
extern default_random_engine dre;
extern uniform_int_distribution<int> uid;