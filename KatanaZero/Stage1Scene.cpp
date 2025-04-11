#include "Stage1Scene.h"
#include "CommonFunction.h"

HRESULT Stage1Scene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	return S_OK;
}

void Stage1Scene::Update()
{
}

void Stage1Scene::Render(HDC hdc)
{
}

void Stage1Scene::Release()
{
}
