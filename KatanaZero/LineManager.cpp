#include "LineManager.h"
#include "Line.h"
#include "ScrollManager.h"

void LineManager::Init()
{
	ZeroMemory(LinePoint, sizeof(FPOINT) * END);
	CurrentLineType = ELineType::Normal;
}

void LineManager::AddLine(float InX, float InY)
{
	// 첫 피킹
	if (!LinePoint[LEFT].x && !LinePoint[LEFT].y)
	{
		LinePoint[LEFT].x = InX;
		LinePoint[LEFT].y = InY;
	}
	// 두번 째 피킹
	else
	{
		LinePoint[RIGHT].x = InX;
		LinePoint[RIGHT].y = InY;

		LineList.push_back(new Line(LinePoint[LEFT], LinePoint[RIGHT], CurrentLineType));

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

bool LineManager::CollisionLine(FPOINT InPos, FLineResult& OutResult, float tolerance)
{
	if (LineList.empty())
		return false;

	// 직선의 방정식으로 라인을 태우자.
	// 캐릭터의 X 값으로 높이를 알 수 있다.
	// 두점을 사용해 직선의 방정식을 구하자.

	Line* Target = nullptr;

	for (auto& iter : LineList)
	{
		// X Offset
		// 해당 라인 안에 있다면
		if (InPos.x >= iter->GetLine().LeftPoint.x && InPos.x < iter->GetLine().RightPoint.x)
		{
			float x1 = iter->GetLine().LeftPoint.x;
			float y1 = iter->GetLine().LeftPoint.y;

			float x2 = iter->GetLine().RightPoint.x;
			float y2 = iter->GetLine().RightPoint.y;

			float y = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;

			float dy = abs(InPos.y - y);
			if (dy <= tolerance)
			{
				OutResult.OutPos.y = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;
				OutResult.LineType = iter->GetLineType();
				Target = iter;
			}
		}
	}

	if (Target == nullptr)
		return false;

	/*
	float x1 = Target->GetLine().LeftPoint.x;
	float y1 = Target->GetLine().LeftPoint.y;

	float x2 = Target->GetLine().RightPoint.x;
	float y2 = Target->GetLine().RightPoint.y;

	float y = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;*/

	return true;
}

void LineManager::Render(HDC hdc)
{
	if (LinePoint[LEFT].x && LinePoint[LEFT].y)
	{
		HPEN hPen = nullptr;

		switch (CurrentLineType)
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

		MoveToEx(hdc, LinePoint[LEFT].x + Scroll.x, LinePoint[LEFT].y + Scroll.y, nullptr);
		LineTo(hdc, g_ptMouse.x, g_ptMouse.y);

		// 사용한 펜을 원래대로 복원
		SelectObject(hdc, hOldPen);

		// 펜 메모리 해제
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


