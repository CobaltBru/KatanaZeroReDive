// main.cpp

#include "config.h"
#include "MainGame.h"
#include "GPImage.h"

#undef new
#include "Reference/Headers/Imgui/imgui.h"
#include "Reference/Headers/Imgui/imgui_impl_win32.h"
#include "Reference/Headers/Imgui/imgui_impl_dx11.h"
#include <tchar.h>
#include "ImGuiManager.h"

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
bool                     g_SwapChainOccluded = false;
UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

HINSTANCE g_hInstance;	// 프로그램 인스턴스 핸들
HWND g_hWnd;
HWND g_hWndDX;
HWND g_hWndParent;
LPCWSTR g_lpszClassName = (LPCWSTR)TEXT("윈도우 API 사용하기");
MainGame g_mainGame;
POINT g_ptMouse;	// 마우스 좌표

random_device rd;
default_random_engine dre;
uniform_int_distribution<int> uid;

//// DirectX
// Forward declarations of helper functions

vector<FloorZone> FloorZones = {
	{ 0.f, 260.f },
	{ 260.f, 340.f },
	{ 340.f, 425.f },
};

// Init

RECT GetRect(int left, int top, int width, int height);
RECT GetRectAtCenter(int x, int y, int width, int height);

// Render
void RenderStar(HDC hdc, int posX, int posY);
void RenderRect(HDC hdc, int x, int y, int width, int height);
void RenderRectAtCenter(HDC hdc, int centerX, int centerY, int width, int height);
void RenderEllipse(HDC hdc, int x, int y, int width, int height);
void RenderEllipseAtCenter(HDC hdc, int centerX, int centerY, int width, int height);

// Collision
bool PointInRect(FPOINT ptMouse, RECT rc);	// ptInRect
bool RectInRect(RECT rc1, RECT rc2);
// Update
void UpdateRect(RECT& rc, FPOINT pt);


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpszCmdParam, int nCmdShow)
{
	
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// 누수 난 메모리 블럭 추적
	//_CrtSetBreakAlloc(101525);
#endif
	
	g_hInstance = hInstance;

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 2068;
	wndClass.cbWndExtra = 2068;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHIELD));

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	GPImage::Init();
	//// 윈도우를 생성하기 위한 데이터 셋팅
	//WNDCLASS wndClass;
	//wndClass.cbClsExtra = 0;
	//wndClass.cbWndExtra = 0;
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	//wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	//wndClass.hInstance = g_hInstance;
	//wndClass.lpfnWndProc = WndProc;		// 함수의 이름은 메모리주소이다.
	//wndClass.lpszClassName = g_lpszClassName;
	//wndClass.lpszMenuName = NULL;
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;	// | : 비트연산자

	//RegisterClass(&wndClass);
	RegisterClassEx(&wndClass);

	//g_hWnd = CreateWindow(g_lpszClassName, g_lpszClassName,
	//	WS_OVERLAPPEDWINDOW, 50, 50, WINSIZE_X, WINSIZE_Y,
	//	NULL, NULL, g_hInstance, NULL);

	RECT rcWindowSize = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect(&rcWindowSize, WS_OVERLAPPEDWINDOW, FALSE);
	int width = rcWindowSize.right - rcWindowSize.left;
	int height = rcWindowSize.bottom - rcWindowSize.top;

	g_hWndParent = CreateWindowEx(0, g_lpszClassName, g_lpszClassName,
		WS_OVERLAPPEDWINDOW, 50, 50, width, height,
		NULL, NULL, g_hInstance, NULL);

	g_hWnd = CreateWindowEx(0, g_lpszClassName, g_lpszClassName,
		WS_CHILD | WS_VISIBLE, 0, 0, WINSIZE_X, height,
		g_hWndParent, NULL, g_hInstance, NULL);

	g_hWndDX = CreateWindowEx(0, g_lpszClassName, g_lpszClassName,
		WS_CHILD | WS_VISIBLE,
		WINSIZE_X, 0, WINSIZE_X + TILEMAPTOOL_X, WINSIZE_Y,
		g_hWndParent, nullptr, hInstance, nullptr);

	ImGuiManager::GetInstance()->Init();

	ShowWindow(g_hWndParent, nCmdShow);

	TimerManager::GetInstance()->Init();
	g_mainGame.Init();

	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TimerManager::GetInstance()->Update();
			g_mainGame.Update();
			g_mainGame.Render();	
		}
	}

	//while (GetMessage(&message, 0, 0, 0))
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	g_mainGame.Release();
	TimerManager::GetInstance()->Release();

	//GDI+ Release
	GPImage::ReleaseLast();
	Gdiplus::GdiplusShutdown(gdiplusToken);

   // Cleanup
	ImGuiManager::GetInstance()->Release();

	return message.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, iMessage, wParam, lParam))
		return true;

	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}
