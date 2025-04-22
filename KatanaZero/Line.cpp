#include "Line.h"
#include "ScrollManager.h"

Line::Line()
{
}

Line::Line(const FPOINT& InLeftPoint, const FPOINT& InRightPoint, const ELineType& InLineType)
	:LineInfo(InLeftPoint, InRightPoint), LineType(InLineType)
{
}

void Line::Render(HDC hdc)
{
	HPEN hPen = nullptr;

	switch (LineType)
	{
	case ELineType::Normal:
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // �ʷ�
		break;
	case ELineType::Wall:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // ����
		break;
	case ELineType::DownLine:
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // ���
		break;
	case ELineType::Ceiling:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0)); // ���
		break;
	}

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // ���� DC�� ���� ����
		
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	MoveToEx(hdc, LineInfo.LeftPoint.x + Scroll.x, LineInfo.LeftPoint.y + Scroll.y, nullptr);
	LineTo(hdc, LineInfo.RightPoint.x + Scroll.x, LineInfo.RightPoint.y + Scroll.y);

	// ����� ���� ������� ����
	SelectObject(hdc, hOldPen);

	// �� �޸� ����
	DeleteObject(hPen);
}

int Line::GetFloorIndex(FPOINT linePoint) const
{
	for (int i = 0; i < FloorZones.size(); ++i)
	{
		if (FloorZones[i].Contains(linePoint.y))
			return i;
	}
	return -1;
}
