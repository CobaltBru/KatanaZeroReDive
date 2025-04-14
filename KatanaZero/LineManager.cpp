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

	// ������ ���������� ������ �¿���.
	// ĳ������ X ������ ���̸� �� �� �ִ�.
	// ������ ����� ������ �������� ������.

	Line* Target = nullptr;

	for (auto& iter : LineList)
	{
		// X Offset
		// �ش� ���� �ȿ� �ִٸ�
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


