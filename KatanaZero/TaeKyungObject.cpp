#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ImageManager.h"

// �׽�Ʈ �ڵ� 
HRESULT TaeKyungObject::Init()
{
    //Image = ImageManager::GetInstance()->FindImage()
    return S_OK;
}



void TaeKyungObject::Update()
{

    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void TaeKyungObject::Render(HDC hdc)
{
    int a = 0;
}


void TaeKyungObject::Release()
{
}