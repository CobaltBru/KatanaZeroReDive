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

	bool CollisionLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);


private:
	list<Line*> LineList;
	FPOINT LinePoint[END];
	ELineType CurrentLineType;
};

