#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>

class Line;
class LineManager : public Singleton<LineManager>
{
private:
	enum Direction { LEFT, RIGHT, END };
public:
	void Init();
	void Render(HDC hdc);
	void Release();

	void AddLine(float InX, float InY, ELineType InLineType = ELineType::Normal);
	void ResetLinePoint();

	bool CollisionLine(FPOINT InPos, FPOINT& OutPos, float tolerance = 3.f);

private:
	list<Line*> LineList;
	FPOINT LinePoint[END];
};

