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

private:
	LINE LineInfo;
	ELineType LineType;
};

