#include "Pawn.h"


Pawn::Pawn()
{
	image = nullptr;
	FrameIndex = 0;
	FrameIndexMax = 0;

	rc = RECT{};
}

HRESULT Pawn::Init()
{
    return S_OK;
}

void Pawn::Release()
{
}

void Pawn::Update()
{
}

void Pawn::Render(HDC hdc)
{
}

void Pawn::MakeSnapShot(void* out)
{
}
