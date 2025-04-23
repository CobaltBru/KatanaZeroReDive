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

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "json.hpp"
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
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다. 
*/
#define WINSIZE_X	1600
#define WINSIZE_Y	900
#define TILEMAPTOOL_X	340
#define TILEMAPTOOL_Y	200

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

typedef struct tagFPOINT
{
	float x;
	float y;
	tagFPOINT& operator +=(const tagFPOINT& fp2)
	{
		x += fp2.x;
		y += fp2.y;
		return *this;
	}
	tagFPOINT& operator *=(const float& f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	tagFPOINT& operator -=(const float& f)
	{
		x -= f;
		y -= f;
		return *this;
	}
	

	tagFPOINT operator *(const float& f)
	{
		tagFPOINT fp;

		fp.x = x * f;
		fp.y = y * f;
		return fp;
	}	
	tagFPOINT operator /(const float& f)
	{
		tagFPOINT fp;

		fp.x = x / f;
		fp.y = y / f;
		return fp;
	}

	tagFPOINT operator -()
	{
		tagFPOINT fp;
		fp.x = -x;
		fp.y = -y;
		return fp;
	}

	bool operator == (const tagFPOINT& fp2)
	{
		return (x == fp2.x && y == fp2.y);
	}
	bool operator != (const tagFPOINT& fp2)
	{
		return (x != fp2.x || y != fp2.y);
	}
	tagFPOINT operator + (const tagFPOINT& fp2) const 
	{
		return tagFPOINT{x+fp2.x, y+fp2.y};
	}
	tagFPOINT operator - (const tagFPOINT& fp2) const
	{
		return tagFPOINT{ x - fp2.x, y - fp2.y };
	}

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

struct FLineResult
{
	FPOINT OutPos;
	ELineType LineType;
	bool IsDiagonalLine;
	bool IsLeft;
};

struct FloorZone
{
	float TopY;
	float BottomY;

	bool Contains(float y)
	{
		return (y > TopY && y <= BottomY);
	}
};



//왼쪽인지 오른쪽인지, Pos, ColliderOffset, ColliderSize, ClassName, 이미지이름
struct FObjectData
{
	int ClsasNameSize;
	int ImageNameSize;
	char* ClassName;	
	char* ImageName;	
	FPOINT Pos;
	FPOINT Offset;
	FPOINT Size;
	bool bLeft;
};

/*
	extern 키워드 : 변수나 함수가 다른 파일에 정의되어 있다 라는
	사실을 알리는 키워드.
*/
extern HWND g_hWnd;
extern HWND g_hWndDX;
extern HWND g_hWndParent;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

// random
extern random_device rd;
extern default_random_engine dre;
extern uniform_int_distribution<int> uid;

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern bool                     g_SwapChainOccluded;
extern UINT                     g_ResizeWidth, g_ResizeHeight;
extern ID3D11RenderTargetView* g_mainRenderTargetView;

// 층 구조
extern vector<FloorZone> FloorZones;