#include "LineManager.h"
#include "Line.h"
#include "ScrollManager.h"
#include "KeyManager.h"

void LineManager::Init()
{
	ZeroMemory(LinePoint, sizeof(FPOINT) * END);
	CurrentLineType = ELineType::Normal;
	CurrentEditState = ELineEditState::Create;
	bStraight = false;
	LineSize = 30.f;
}

void LineManager::Update()
{
	// �׽�Ʈ �ڵ�
	/*if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
	{
		if (CurrentEditState == ELineEditState::Create || CurrentEditState == ELineEditState::Adjust)
		{
			const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
			AddLine(g_ptMouse.x - Scroll.x, g_ptMouse.y - Scroll.y);
		}
		else if (CurrentEditState == ELineEditState::Eraser)
			DestroyLine();
	}*/

	//if (KeyManager::GetInstance()->IsOnceKeyDown('1'))
	//	CurrentEditState = ELineEditState::Create;
	//// ����
	//else if (KeyManager::GetInstance()->IsOnceKeyDown('2'))
	//	CurrentEditState = ELineEditState::Adjust;
	//else if (KeyManager::GetInstance()->IsOnceKeyDown('3'))
	//{
	//	CurrentEditState = ELineEditState::Eraser;
	//	ResetLinePoint();
	//}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD0))
		ResetLinePoint();
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD1))
	//	SetLineType(ELineType::Normal);
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD2))
	//	SetLineType(ELineType::Wall);
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD3))
	//	SetLineType(ELineType::DownLine);
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD4))
	//	SetLineType(ELineType::Ceiling);
	////����
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F11))
	//	SaveFile();
	////�ҷ�����
	//else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F12))
	//	LoadFile();
}

void LineManager::AddLine(float InX, float InY)
{
	switch (CurrentEditState)
	{
	case ELineEditState::Create:
		CreateLine(InX, InY);
		break;
	case ELineEditState::Adjust:
		AdjustLine(InX, InY);
		break;
	}
}

void LineManager::ResetLinePoint()
{
	LinePoint[LEFT].x = 0.f;
	LinePoint[LEFT].y = 0.f;
	LinePoint[RIGHT].x = 0.f;
	LinePoint[RIGHT].y = 0.f;
}

void LineManager::DestroyLine()
{
	// ���õ� ���� ����
	// ���� ����� ������ ������
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	FPOINT pt = { g_ptMouse.x - Scroll.x,g_ptMouse.y - Scroll.y };

	for (int i = 0; i < (int)ELineType::End; ++i)
	{
		for (auto iter = LineList[i].begin(); iter != LineList[i].end();)
		{
			if (i == (int)ELineType::Wall)
			{
				if (pt.y + LineSize >= min((*iter)->GetLine().RightPoint.y, (*iter)->GetLine().LeftPoint.y) && pt.y - LineSize <= max((*iter)->GetLine().RightPoint.y, (*iter)->GetLine().LeftPoint.y))
				{
					if (pt.x + LineSize >= (*iter)->GetLine().LeftPoint.x && pt.x - LineSize < (*iter)->GetLine().LeftPoint.x)
					{
						delete (*iter);
						iter = LineList[i].erase(iter);
						return;
					}
				}
			}
			else
			{
				if (pt.x >= (*iter)->GetLine().LeftPoint.x && pt.x <= (*iter)->GetLine().RightPoint.x)
				{
					float x1 = (*iter)->GetLine().LeftPoint.x;
					float y1 = (*iter)->GetLine().LeftPoint.y;

					float x2 = (*iter)->GetLine().RightPoint.x;
					float y2 = (*iter)->GetLine().RightPoint.y;

					float dY = ((y2 - y1) / (x2 - x1)) * (pt.x - x1) + y1;
					if (pt.y - LineSize <= dY && pt.y + LineSize >= dY)
					{
						delete (*iter);
						iter = LineList[i].erase(iter);
						return;
					}
				}
			}
			++iter;
		}
	}
}

void LineManager::DestroyAllLine()
{
	for (int i = 0; i < (int)ELineType::End; ++i)
	{
		for (auto& iter : LineList[i])
			delete iter;

		LineList[i].clear();
	}
}

bool LineManager::CollisionLine(FPOINT InPos, FLineResult& OutResult, float tolerance)
{
	if (LineList[(int)ELineType::Normal].empty() && LineList[(int)ELineType::DownLine].empty())
		return false;

	const float HalfTolerance = tolerance * 0.5f;
	const float ObjectBottom = InPos.y + HalfTolerance;

	Line* Target = nullptr;

	float MinY = FLT_MAX;
	for (int i = 0; i < (int)ELineType::DownLine + 1; ++i)
	{
		for (auto& iter : LineList[i])
		{
			if (InPos.x >= iter->GetLine().LeftPoint.x && InPos.x <= iter->GetLine().RightPoint.x)
			{
				float x1 = iter->GetLine().LeftPoint.x;
				float y1 = iter->GetLine().LeftPoint.y;

				float x2 = iter->GetLine().RightPoint.x;
				float y2 = iter->GetLine().RightPoint.y;

				float dY = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;

				if (MinY > abs(dY - ObjectBottom))
				{
					MinY = abs(dY - ObjectBottom);
					OutResult.OutPos.y = dY - HalfTolerance;
					Target = iter;
				}
			}
		}
	}

	if (Target == nullptr)
		return false;

	return true;
}

bool LineManager::CollisionLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, float tolerance, bool IsDown)
{
	if (LineList[(int)ELineType::Normal].empty() && LineList[(int)ELineType::DownLine].empty())
		return false;

	// ������ ���������� ������ �¿���.
	// ĳ������ X ������ ���̸� �� �� �ִ�.
	// ������ ����� ������ �������� ������.

	const float HalfTolerance = tolerance * 0.5f;
	const float ObjectBottom = InPos.y + HalfTolerance;
	const float ObjectTop = InPos.y - HalfTolerance;

	const float LastObjectBottom = InLastPos.y + HalfTolerance;

	float NormalY = 0.f;
	float DownY = 0.f;
	Line* NormalTarget = nullptr;
	Line* DownTarget = nullptr;

	for (int i = 0; i < (int)ELineType::DownLine + 1; ++i)
	{
		if (IsDown && i == (int)ELineType::DownLine)
			break;

		for (auto& iter : LineList[i])
		{
			// X Offset
			// �ش� ���� �ȿ� �ִٸ�
			if (InPos.x >= iter->GetLine().LeftPoint.x && InPos.x <= iter->GetLine().RightPoint.x)
			{
				float x1 = iter->GetLine().LeftPoint.x;
				float y1 = iter->GetLine().LeftPoint.y;

				float x2 = iter->GetLine().RightPoint.x;
				float y2 = iter->GetLine().RightPoint.y;

				float dY = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;


				// �̰͸� ��Ʋ�ɷȴ�.. ���� ������ ���� �� ����~
				/*&& ((iter->GetLine().LeftPoint.y == iter->GetLine().RightPoint.y && LastObjectBottom <= dY) || (iter->GetLine().LeftPoint.y != iter->GetLine().RightPoint.y))*/
				//&& (iter->GetLine().LeftPoint.y == iter->GetLine().RightPoint.y || (iter->GetLine().LeftPoint.y != iter->GetLine().RightPoint.y&&))
				//&& LastObjectBottom <= dY /*((i != (int)ELineType::DownLine) || LastObjectBottom <= dY)*/
				if (ObjectBottom >= dY && ObjectTop < dY && ObjectBottom >= dY - HalfTolerance && ObjectBottom <= dY + HalfTolerance
					&& ((iter->GetLine().LeftPoint.y == iter->GetLine().RightPoint.y && LastObjectBottom <= dY) || (iter->GetLine().LeftPoint.y != iter->GetLine().RightPoint.y)))
				{
					if (i == (int)ELineType::Normal)
					{
						NormalY = dY - HalfTolerance;
						NormalTarget = iter;
					}
					else
					{
						DownY = dY - HalfTolerance;
						DownTarget = iter;
					}
				}
			}
		}
	}

	if (NormalTarget == nullptr && DownTarget == nullptr)
		return false;

	// ����
	if (NormalTarget == nullptr || DownTarget == nullptr)
	{
		Line* Target = DownTarget == nullptr ? NormalTarget : DownTarget;

		OutResult.OutPos.y = DownTarget == nullptr ? NormalY : DownY;
		OutResult.LineType = Target->GetLineType();

		OutResult.IsDiagonalLine = (Target->GetLine().LeftPoint.y != Target->GetLine().RightPoint.y);

		return true;
	}

	//�ΰ��� ���� �� ������
	const float NormalDY = abs(ObjectBottom - (NormalY + HalfTolerance));
	const float DownDY = abs(ObjectBottom - (DownY + HalfTolerance));
	if (NormalDY >= DownDY)
	{
		OutResult.OutPos.y = DownY;
		OutResult.LineType = DownTarget->GetLineType();
		OutResult.IsDiagonalLine = (DownTarget->GetLine().LeftPoint.y != DownTarget->GetLine().RightPoint.y);
	}
	else
	{
		OutResult.OutPos.y = NormalY;
		OutResult.LineType = NormalTarget->GetLineType();
		OutResult.IsDiagonalLine = (NormalTarget->GetLine().LeftPoint.y != NormalTarget->GetLine().RightPoint.y);
	}

	return true;
}

bool LineManager::CollisionLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, bool IsGround, float tolerance, bool IsDown)
{
	if (LineList[(int)ELineType::Normal].empty() && LineList[(int)ELineType::DownLine].empty())
		return false;

	// ������ ���������� ������ �¿���.
	// ĳ������ X ������ ���̸� �� �� �ִ�.
	// ������ ����� ������ �������� ������.

	if (IsGround)
	{
		const float HalfTolerance = tolerance * 0.5f;
		const float ObjectBottom = InPos.y + HalfTolerance;
		const float ObjectTop = InPos.y - HalfTolerance;

		const float LastObjectBottom = InLastPos.y + HalfTolerance;

		float NormalY = 0.f;
		float DownY = 0.f;
		Line* NormalTarget = nullptr;
		Line* DownTarget = nullptr;

		for (int i = 0; i < (int)ELineType::DownLine + 1; ++i)
		{
			if (IsDown && i == (int)ELineType::DownLine)
				break;

			for (auto& iter : LineList[i])
			{
				// X Offset
				// �ش� ���� �ȿ� �ִٸ�
				if (InPos.x >= iter->GetLine().LeftPoint.x && InPos.x <= iter->GetLine().RightPoint.x)
				{
					float x1 = iter->GetLine().LeftPoint.x;
					float y1 = iter->GetLine().LeftPoint.y;

					float x2 = iter->GetLine().RightPoint.x;
					float y2 = iter->GetLine().RightPoint.y;

					float dY = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;


					//�밢��
					if (iter->GetLine().LeftPoint.y != iter->GetLine().RightPoint.y && InPos.x != InLastPos.x
						&& ObjectBottom < dY && OutResult.IsDiagonalLine)
					{
						if (i == (int)ELineType::Normal)
						{
							NormalY = dY - HalfTolerance;
							NormalTarget = iter;
						}
						else
						{
							DownY = dY - HalfTolerance;
							DownTarget = iter;
						}
					}
					else
					{
						if (ObjectBottom >= dY && ObjectTop < dY && ObjectBottom >= dY - HalfTolerance && ObjectBottom <= dY + HalfTolerance
							/*&& ((i != (int)ELineType::DownLine) || LastObjectBottom <= dY)*/)
						{
							if (i == (int)ELineType::Normal)
							{
								NormalY = dY - HalfTolerance;
								NormalTarget = iter;
							}
							else
							{
								DownY = dY - HalfTolerance;
								DownTarget = iter;
							}
						}
					}
				}
			}
		}

		if (NormalTarget == nullptr && DownTarget == nullptr)
			return false;

		// ����
		if (NormalTarget == nullptr || DownTarget == nullptr)
		{
			Line* Target = DownTarget == nullptr ? NormalTarget : DownTarget;

			OutResult.OutPos.y = DownTarget == nullptr ? NormalY : DownY;
			OutResult.LineType = Target->GetLineType();

			OutResult.IsDiagonalLine = (Target->GetLine().LeftPoint.y != Target->GetLine().RightPoint.y);

			return true;
		}

		//�ΰ��� ���� �� ������
		const float NormalDY = abs(ObjectBottom - (NormalY + HalfTolerance));
		const float DownDY = abs(ObjectBottom - (DownY + HalfTolerance));
		if (NormalDY >= DownDY)
		{
			OutResult.OutPos.y = DownY;
			OutResult.LineType = DownTarget->GetLineType();
			OutResult.IsDiagonalLine = (DownTarget->GetLine().LeftPoint.y != DownTarget->GetLine().RightPoint.y);
		}
		else
		{
			OutResult.OutPos.y = NormalY;
			OutResult.LineType = NormalTarget->GetLineType();
			OutResult.IsDiagonalLine = (NormalTarget->GetLine().LeftPoint.y != NormalTarget->GetLine().RightPoint.y);
		}

		return true;
	}
	else
		return CollisionLine(InPos, InLastPos, OutResult, tolerance, IsDown);
}

bool LineManager::CollisionWallLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, FPOINT InSize)
{
	if (LineList[(int)ELineType::Wall].empty())
		return false;

	//Wall�� ������ ����

	const FPOINT HalfSize = { InSize.x * 0.5f, InSize.y * 0.5f };

	// Object Rect
	RECT ObjectRC;
	ObjectRC.left = InPos.x - HalfSize.x;
	ObjectRC.right = InPos.x + HalfSize.x;
	ObjectRC.top = InPos.y - HalfSize.y;
	ObjectRC.bottom = InPos.y + HalfSize.y;

	Line* Target = nullptr;

	for (auto& iter : LineList[(int)ELineType::Wall])
	{
		// �ش� �����ȿ� �ִ�.		
		if (ObjectRC.bottom > min(iter->GetLine().RightPoint.y, iter->GetLine().LeftPoint.y) && ObjectRC.top < max(iter->GetLine().RightPoint.y, iter->GetLine().LeftPoint.y))
		{
			// ���ʿ� ������ ���� �� 
			// �����̱� ������ x������ ������.			

			if (ObjectRC.right >= iter->GetLine().LeftPoint.x && ObjectRC.left <= iter->GetLine().LeftPoint.x)
			{
				const float LeftLength = abs(ObjectRC.left - iter->GetLine().LeftPoint.x);
				const float RightLength = abs(ObjectRC.right - iter->GetLine().LeftPoint.x);

				if (InPos.x >= InLastPos.x && LeftLength > RightLength)
				{
					OutResult.OutPos.x = iter->GetLine().LeftPoint.x - HalfSize.x;					
					OutResult.LineType = iter->GetLineType();
					Target = iter;

					OutResult.IsLeft = false;
				}
				else if (InPos.x <= InLastPos.x && LeftLength < RightLength)
				{					
					OutResult.OutPos.x = iter->GetLine().LeftPoint.x + HalfSize.x;
					OutResult.LineType = iter->GetLineType();
					Target = iter;

					OutResult.IsLeft = true;
				}
			}
		}
	}

	if (Target == nullptr)
		return false;

	return true;
}

bool LineManager::CollisionCeilingLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, float tolerance)
{
	if (LineList[(int)ELineType::Ceiling].empty())
		return false;

	// ������ ���������� ������ �¿���.
	// ĳ������ X ������ ���̸� �� �� �ִ�.
	// ������ ����� ������ �������� ������.
	const float HalfTolerance = tolerance * 0.5f;
	const float ObjectTop = InPos.y - HalfTolerance;
	const float LastObjectTop = InLastPos.y - HalfTolerance;


	Line* Target = nullptr;

	//ObjectBottom >= dY && ObjectTop < dY && ObjectBottom >= dY - HalfTolerance && ObjectBottom <= dY + HalfTolerance

	for (auto& iter : LineList[(int)ELineType::Ceiling])
	{
		// X Offset
		// �ش� ���� �ȿ� �ִٸ�
		if (InPos.x >= iter->GetLine().LeftPoint.x && InPos.x <= iter->GetLine().RightPoint.x)
		{
			float x1 = iter->GetLine().LeftPoint.x;
			float y1 = iter->GetLine().LeftPoint.y;

			float x2 = iter->GetLine().RightPoint.x;
			float y2 = iter->GetLine().RightPoint.y;

			float dY = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;

			if (ObjectTop <= dY && LastObjectTop > dY)
			{
				OutResult.OutPos.y = dY + HalfTolerance;
				OutResult.LineType = iter->GetLineType();
				Target = iter;
			}
		}
	}

	if (Target == nullptr)
		return false;

	return true;
}

bool LineManager::CollisionDownLine(FPOINT InPos, FLineResult& OutResult, float tolerance)
{
	if (LineList[(int)ELineType::DownLine].empty())
		return false;

	const float HalfTolerance = tolerance * 0.5f;
	Line* Target = nullptr;

	for (auto& iter : LineList[(int)ELineType::DownLine])
	{
		// X Offset
		// �ش� ���� �ȿ� �ִٸ�
		if (InPos.x >= iter->GetLine().LeftPoint.x && InPos.x <= iter->GetLine().RightPoint.x)
		{
			float x1 = iter->GetLine().LeftPoint.x;
			float y1 = iter->GetLine().LeftPoint.y;

			float x2 = iter->GetLine().RightPoint.x;
			float y2 = iter->GetLine().RightPoint.y;

			float y = ((y2 - y1) / (x2 - x1)) * (InPos.x - x1) + y1;

			float ObjectBottom = InPos.y + HalfTolerance;

			if (ObjectBottom >= y - HalfTolerance && ObjectBottom <= y + HalfTolerance)
			{
				OutResult.OutPos.y = y - HalfTolerance;
				OutResult.LineType = iter->GetLineType();
				Target = iter;
			}
		}
	}

	if (Target == nullptr)
		return false;

	return true;
}

HRESULT LineManager::SaveFile(LPCWSTR InSavePath)
{
	HANDLE hFile = CreateFile(
		InSavePath, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"LineManager SaveFile Failed.", TEXT("���"), MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;
	for (int i = 0; i < (int)ELineType::End; ++i)
	{
		LINE SaveLine;
		ELineType SaveLineType;
		for (auto& iter : LineList[i])
		{
			SaveLine = iter->GetLine();
			SaveLineType = iter->GetLineType();
			WriteFile(hFile, &SaveLine, sizeof(LINE), &dwByte, NULL);
			WriteFile(hFile, &SaveLineType, sizeof(ELineType), &dwByte, NULL);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT LineManager::LoadFile(LPCWSTR InLoadPath)
{
	DestroyAllLine();
	
	//LPCWSTR
	HANDLE hFile = CreateFile(
		InLoadPath, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"LineManager LoadFile Failed.", TEXT("���"), MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;

	while (true)
	{
		LINE SaveLine;
		ELineType SaveLineType;

		ReadFile(hFile, &SaveLine, sizeof(LINE), &dwByte, NULL);
		ReadFile(hFile, &SaveLineType, sizeof(ELineType), &dwByte, NULL);

		if (dwByte == 0)
			break;

		LineList[(int)SaveLineType].push_back(new Line(SaveLine.LeftPoint, SaveLine.RightPoint, SaveLineType));
	}

	CloseHandle(hFile);

	return S_OK;
}

void LineManager::CreateLine(int InX, int InY)
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
		float LeftX = LinePoint[LEFT].x;
		float LeftY = LinePoint[LEFT].y;

		// Wall�� ������ �� ������
		if (CurrentLineType == ELineType::Wall)
		{
			if (LinePoint[LEFT].y < InY)
			{
				LinePoint[RIGHT].y = LinePoint[LEFT].y;

				LinePoint[LEFT].y = InY;
			}
			else
				LinePoint[RIGHT].y = InY;

			LinePoint[RIGHT].x = LinePoint[LEFT].x;
		}
		else
		{
			if (LinePoint[LEFT].x > InX)
			{
				LinePoint[RIGHT].x = LinePoint[LEFT].x;
				LinePoint[RIGHT].y = LinePoint[LEFT].y;

				LinePoint[LEFT].x = InX;
				LinePoint[LEFT].y = InY;
			}
			else
			{
				LinePoint[RIGHT].x = InX;
				LinePoint[RIGHT].y = InY;
			}

			// ���� ����
			if (bStraight)
			{
				LinePoint[LEFT].y = LeftY;
				LinePoint[RIGHT].y = LeftY;
			}
		}

		LineList[(int)CurrentLineType].push_back(new Line(LinePoint[LEFT], LinePoint[RIGHT], CurrentLineType));

		if (LeftX > InX)
		{
			LinePoint[LEFT].x = InX;
			LinePoint[LEFT].y = InY;
		}
		else
		{
			LinePoint[LEFT].x = LinePoint[RIGHT].x;
			LinePoint[LEFT].y = LinePoint[RIGHT].y;
		}
	}
}

void LineManager::AdjustLine(float InX, float InY)
{
	//�����ؼ� �ֺ� ������ �ִٸ� �� ���ΰ� ��������.

	for (int i = 0; i < (int)ELineType::End; ++i)
	{
		for (auto& iter : LineList[i])
		{
			if (i == (int)ELineType::Wall)
			{
				if (InY + LineSize >= min(iter->GetLine().RightPoint.y, iter->GetLine().LeftPoint.y) && InY - LineSize <= max(iter->GetLine().RightPoint.y, iter->GetLine().LeftPoint.y))
				{
					if (InX + LineSize >= iter->GetLine().LeftPoint.x && InX - LineSize < iter->GetLine().LeftPoint.x)
					{
						const float LeftLength = iter->GetLine().LeftPoint.y - InY;
						const float RightLength = InY - iter->GetLine().RightPoint.y;

						CreateLine(LeftLength > RightLength ? iter->GetLine().RightPoint.x : iter->GetLine().LeftPoint.x,
							LeftLength > RightLength ? iter->GetLine().RightPoint.y : iter->GetLine().LeftPoint.y);
						return;
					}
				}
			}
			else
			{
				if (InX + LineSize >= iter->GetLine().LeftPoint.x && InX - LineSize <= iter->GetLine().RightPoint.x)
				{
					float x1 = iter->GetLine().LeftPoint.x;
					float y1 = iter->GetLine().LeftPoint.y;

					float x2 = iter->GetLine().RightPoint.x;
					float y2 = iter->GetLine().RightPoint.y;

					float dY = ((y2 - y1) / (x2 - x1)) * (InX - x1) + y1;
					if (InY - LineSize <= dY && InY + LineSize >= dY)
					{
						const float LeftLength = InX - iter->GetLine().LeftPoint.x;
						const float RightLength = iter->GetLine().RightPoint.x - InX;

						CreateLine(LeftLength > RightLength ? iter->GetLine().RightPoint.x : iter->GetLine().LeftPoint.x,
							LeftLength > RightLength ? iter->GetLine().RightPoint.y : iter->GetLine().LeftPoint.y);
						return;
					}
				}
			}
		}
	}

	CreateLine(InX, InY);
}

pair<FPOINT, FPOINT> LineManager::FindNearestSlope(const FPOINT& targetPos, int fromFloor, int toFloor) const
{
	FPOINT nearestEntry = { 0.f, 0.f };
	FPOINT nearestExit = { 0.f, 0.f };
	float nearestDist = FLT_MAX;

	for (auto* line : LineList[(int)ELineType::Normal])
	{
		if (!line->IsDiagonalLine()) continue;

		const auto& Lpos = line->GetLine().LeftPoint;
		const auto& Rpos = line->GetLine().RightPoint;
		int floorL = line->GetFloorIndex(Lpos);
		int floorR = line->GetFloorIndex(Rpos);
		bool matches =
			(floorL == fromFloor && floorR == toFloor) ||
			(floorR == fromFloor && floorL == toFloor);
		if (!matches) continue;
		FPOINT entry = (floorL == fromFloor) ? Lpos : Rpos;
		FPOINT exit = (floorL == fromFloor) ? Rpos : Lpos;
		float dx = fabs(entry.x - targetPos.x);
		if (dx < nearestDist)
		{
			nearestDist = dx;
			nearestEntry = entry;
			nearestExit = exit;
		}
	}
	return make_pair(nearestEntry, nearestExit);
}

void LineManager::Render(HDC hdc)
{
	bStraight = KeyManager::GetInstance()->IsStayKeyDown(VK_SHIFT);

	if (LinePoint[LEFT].x && LinePoint[LEFT].y)
	{
		HPEN hPen = nullptr;
		POINT pt;
		pt = g_ptMouse;

		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

		if (bStraight && CurrentLineType != ELineType::Wall)
			pt.y = LinePoint[LEFT].y + Scroll.y;

		switch (CurrentLineType)
		{
		case ELineType::Normal:
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // �ʷ�
			break;
		case ELineType::Wall:
			hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // ����
			pt.x = LinePoint[LEFT].x + Scroll.x;
			break;
		case ELineType::DownLine:
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // ���
			break;
		case ELineType::Ceiling:
			hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0)); // ���
			break;
		}

		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // ���� DC�� ���� ����	

		MoveToEx(hdc, LinePoint[LEFT].x + Scroll.x, LinePoint[LEFT].y + Scroll.y, nullptr);
		LineTo(hdc, pt.x, pt.y);

		// ����� ���� ������� ����
		SelectObject(hdc, hOldPen);

		// �� �޸� ����
		DeleteObject(hPen);
	}
	else if (CurrentEditState == ELineEditState::Eraser)
	{
		Ellipse(hdc, (int)(g_ptMouse.x - LineSize), (int)(g_ptMouse.y - LineSize), (int)(g_ptMouse.x + LineSize), (int)(g_ptMouse.y + LineSize));
	}

	for (int i = 0; i < (int)ELineType::End; ++i)
	{
		for (auto& iter : LineList[i])
			iter->Render(hdc);
	}
}

void LineManager::Release()
{
	for (int i = 0; i < (int)ELineType::End; ++i)
	{
		for (auto& iter : LineList[i])
			delete iter;

		LineList[i].clear();
	}

	ReleaseInstance();
}
