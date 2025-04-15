#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>

struct FLineResult
{
	FPOINT OutPos;
	ELineType LineType;
};

class Line;
class LineManager : public Singleton<LineManager>
{
private:
	enum Direction { LEFT, RIGHT, END };
public:
	void Init();
	void Render(HDC hdc);
	void Release();

	void SetLineType(ELineType InLineType) { CurrentLineType = InLineType; }
	void AddLine(float InX, float InY);
	void ResetLinePoint();

	bool CollisionLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f, bool IsDown = false);
	bool CollisionWallLine(FPOINT InPos, FLineResult& OutResult, FPOINT InSize);
	bool CollisionCeilingLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);
	bool CollisionDownLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);

	HRESULT SaveFile();
	HRESULT LoadFile();
	
private:
	list<Line*> LineList[(int)ELineType::End];
	FPOINT LinePoint[END];
	ELineType CurrentLineType;
	bool bStraight;
};

