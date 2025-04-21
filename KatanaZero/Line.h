#pragma once
#include "config.h"

class Line
{
public:
	Line();
	Line(const FPOINT& InLeftPoint, const FPOINT& InRightPoint, const ELineType& InLineType);
	~Line() = default;

	void Render(HDC hdc);

	const LINE& GetLine() const { return LineInfo; }
	const ELineType& GetLineType() const { return LineType; }
	//대각선인지(비탈길인지) 정보 알아야함
	bool IsDiagonalLine() const { return (LineInfo.LeftPoint.y != LineInfo.RightPoint.y); }
	int GetFloorIndex(FPOINT linePoint) const;
private:
	LINE LineInfo;
	ELineType LineType;
};

