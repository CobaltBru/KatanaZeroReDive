// CommonFunction.h
/*
	inline: 컴파일러에게 함수 호출을 최적화 하도록 요청하는 키워드
	일반적인 함수들은 함수 호출 시에 매개변수를 스택메모리 저장, 
	함수 시작주소를 호출, 반환주소로 이동
	// main.cpp
	int result = 3 + 6;

	inline int Add(int a, int b)
	{
		return a + b;
	}
*/
#pragma once
#include "config.h"

inline RECT GetRect(int left, int top, int width, int height)
{
	RECT rc{ left, top, left + width, top + height };
	return rc;
}

inline RECT GetRectAtCenter(int x, int y, int width, int height)
{
	RECT rc{ x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2) };
	return rc;
}

inline void RenderStar(HDC hdc, int posX, int posY)
{
	MoveToEx(hdc, posX + 60, posY + 20, NULL);
	LineTo(hdc, posX - 60, posY + 20);
	LineTo(hdc, posX + 40, posY - 60);
	LineTo(hdc, posX, posY + 60);
	LineTo(hdc, posX - 40, posY - 60);
	LineTo(hdc, posX + 60, posY + 20);
}

// 좌상단 기준
inline void RenderRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

// 가운데 기준
inline void RenderRectAtCenter(HDC hdc, int centerX, int centerY, int width, int height)
{
	Rectangle(hdc, centerX - (width / 2), centerY - (height / 2),
		centerX + (width / 2), centerY + (height / 2));
}

// 좌상단 기준
inline void RenderEllipse(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

// 가운데 기준
inline void RenderEllipseAtCenter(HDC hdc, int centerX, int centerY, int width, int height)
{
	Ellipse(hdc, centerX - (width / 2), centerY - (height / 2),
		centerX + (width / 2), centerY + (height / 2));
}

inline bool PointInRect(POINT ptMouse, RECT rc)
{
	if (ptMouse.x < rc.left || ptMouse.x > rc.right
		|| ptMouse.y < rc.top || ptMouse.y > rc.bottom)
	{
		return false;
	}
	return true;
}

inline bool RectInRect(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left || rc1.left > rc2.right
		|| rc1.top > rc2.bottom || rc1.bottom < rc2.top)
	{
		return false;
	}
	return true;
}

inline void UpdateRect(RECT& rc, FPOINT pt)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	rc.left = pt.x - (width / 2);
	rc.right = rc.left + width;
	rc.top = pt.y - (height / 2);
	rc.bottom = rc.top + height;
}

inline float GetAngle(FPOINT start, FPOINT end)
{
	float x = end.x - start.x;
	float y = end.y - start.y;

	 return -atan2f(y, x);	// 라디안
}

inline float GetDistance(FPOINT p1, FPOINT p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	return sqrtf(x * x + y * y);
}

inline FPOINT RotatePoint(const FPOINT& p, double angleRad) {
	FPOINT rotated;
	rotated.x = p.x * std::cos(angleRad) - p.y * std::sin(angleRad);
	rotated.y = p.x * std::sin(angleRad) + p.y * std::cos(angleRad);
	return rotated;
}

inline void SetClientRect(HWND hWnd, int width, int height)
{
	RECT rc;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	DWORD exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	SetRect(&rc, 0, 0, width, height);
	AdjustWindowRectEx(&rc, style, GetMenu(hWnd) != NULL, exStyle);
	if (style & WS_VSCROLL)
		rc.right += GetSystemMetrics(SM_CXVSCROLL);
	if (style & WS_HSCROLL)
		rc.bottom += GetSystemMetrics(SM_CYHSCROLL);

	SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, 
		rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

inline void DrawRoundRect(Gdiplus::Graphics* graphics, FPOINT pos, float width, float height, Gdiplus::Color fillColor)
{
	int cornerRadius = 4;
	Gdiplus::Rect rect(pos.x, pos.y, width, height);

	Gdiplus::GraphicsPath path;

	path.AddArc(rect.X, rect.Y, cornerRadius * 2, cornerRadius * 2, 180, 90);
	path.AddArc(rect.X + rect.Width - cornerRadius * 2, rect.Y, cornerRadius * 2, cornerRadius * 2, 270, 90);
	path.AddArc(rect.X + rect.Width - cornerRadius * 2, rect.Y + rect.Height - cornerRadius * 2, cornerRadius * 2, cornerRadius * 2, 0, 90);
	path.AddArc(rect.X, rect.Y + rect.Height - cornerRadius * 2, cornerRadius * 2, cornerRadius * 2, 90, 90);
	path.CloseFigure();

	Gdiplus::SolidBrush brush(fillColor);
	graphics->FillPath(&brush, &path);
}

inline float GetLength(const FPOINT& InPoint)
{
	return sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y);
}

inline void Normalize(FPOINT& InPoint)
{
	const float Length = GetLength(InPoint);
	if(InPoint.x != 0.f)
		InPoint.x /= Length;

	if (InPoint.y != 0.f)
		InPoint.y /= Length;
}

inline std::wstring Utf8ToWstring(const std::string& utf8) {
	if (utf8.empty()) return {};
	int size = ::MultiByteToWideChar(
		CP_UTF8, 0,
		utf8.data(), (int)utf8.size(),
		nullptr, 0
	);
	std::wstring wstr;
	wstr.resize(size);
	::MultiByteToWideChar(
		CP_UTF8, 0,
		utf8.data(), (int)utf8.size(),
		&wstr[0], size
	);
	return wstr;
}

inline float SoftStart(float t)
{
	return t * t;
}
inline float SoftEnd(float t)
{
	return 1 - (1 - t) * (1 - t);
}
inline float SoftStartEnd(float t)
{
	return t < 0.5f
		? 2 * t * t
		: 1 - 2 * (1 - t) * (1 - t);
}