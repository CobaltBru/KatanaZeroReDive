#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>

enum class ELineEditState : uint8_t
{
	Create,
	Eraser,
	Adjust,
	End
};

class Line;
class LineManager : public Singleton<LineManager>
{
private:
	enum Direction { LEFT, RIGHT, END };
public:
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	ELineEditState GetCurrentEditState() const { return CurrentEditState; }

	void SetLineType(ELineType InLineType) { CurrentLineType = InLineType; }
	void SetLineEditState(ELineEditState InLineEditState) { CurrentEditState = InLineEditState; }
	void AddLine(float InX, float InY);
	void ResetLinePoint();
	void DestroyLine();
	void DestroyAllLine();

	bool CollisionLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);
	bool CollisionLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, float tolerance = 3.f, bool IsDown = false);
	bool CollisionLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, bool IsGround, float tolerance = 3.f, bool IsDown = false);
	bool CollisionWallLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, FPOINT InSize);
	bool CollisionCeilingLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, float tolerance = 3.f);
	bool CollisionDownLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);

	HRESULT SaveFile(LPCWSTR InSavePath);
	HRESULT LoadFile(LPCWSTR InLoadPath);

	void CreateLine(int InX, int InY);
	void AdjustLine(float InX, float InY);

	// 적 AI용 비탈길 찾기
	pair<FPOINT, FPOINT> FindNearestSlope(const FPOINT& targetPos, int fromFloor, int toFloor) const;
private:
	list<Line*> LineList[(int)ELineType::End];
	FPOINT LinePoint[END];
	ELineType CurrentLineType;
	bool bStraight;
	ELineEditState CurrentEditState;
	float LineSize;
};

