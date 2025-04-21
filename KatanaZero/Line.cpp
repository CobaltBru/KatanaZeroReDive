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
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // 초록
		break;
	case ELineType::Wall:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 빨강
		break;
	case ELineType::DownLine:
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // 블루
		break;
	case ELineType::Ceiling:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0)); // 노랑
		break;
	}

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // 현재 DC에 펜을 설정
		
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	MoveToEx(hdc, LineInfo.LeftPoint.x + Scroll.x, LineInfo.LeftPoint.y + Scroll.y, nullptr);
	LineTo(hdc, LineInfo.RightPoint.x + Scroll.x, LineInfo.RightPoint.y + Scroll.y);

	// 사용한 펜을 원래대로 복원
	SelectObject(hdc, hOldPen);

	// 펜 메모리 해제
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
