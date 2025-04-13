#include "LineManager.h"
#include "Line.h"
#include "ScrollManager.h"

void LineManager::Init()
{
	ZeroMemory(LinePoint, sizeof(FPOINT) * END);
}

void LineManager::AddLine(float InX, float InY, ELineType InLineType)
{
	// ù ��ŷ
	if (!LinePoint[LEFT].x && !LinePoint[LEFT].y)
	{
		LinePoint[LEFT].x = InX;
		LinePoint[LEFT].y = InY;
	}
	// �ι� ° ��ŷ
	else
	{
		LinePoint[RIGHT].x = InX;
		LinePoint[RIGHT].y = InY;

		LineList.push_back(new Line(LinePoint[LEFT], LinePoint[RIGHT], InLineType));

		LinePoint[LEFT].x = LinePoint[RIGHT].x;
		LinePoint[LEFT].y = LinePoint[RIGHT].y;
	}
}

void LineManager::ResetLinePoint()
{
	LinePoint[LEFT].x = 0.f;
	LinePoint[LEFT].y = 0.f;
	LinePoint[RIGHT].x = 0.f;
	LinePoint[RIGHT].y = 0.f;
}

void LineManager::Render(HDC hdc)
{
	if (LinePoint[LEFT].x && LinePoint[LEFT].y)
	{
		HPEN hPen = nullptr;
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // �ʷ�
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // ���� DC�� ���� ����
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

		MoveToEx(hdc, LinePoint[LEFT].x + Scroll.x, LinePoint[LEFT].y + Scroll.y, nullptr);
		LineTo(hdc, g_ptMouse.x, g_ptMouse.y);

		// ����� ���� ������� ����
		SelectObject(hdc, hOldPen);

		// �� �޸� ����
		DeleteObject(hPen);
	}

	for (auto& iter : LineList)
		iter->Render(hdc);
}

void LineManager::Release()
{
	for (auto& iter : LineList)
		delete iter;
	LineList.clear();

	ReleaseInstance();
}


