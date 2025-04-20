#include "GameObject.h"

HRESULT GameObject::Init()
{
	return S_OK;
}

HRESULT GameObject::Init(FPOINT Pos)
{
	return S_OK;
}

void GameObject::Release()
{
}

void GameObject::Update()
{
}

void GameObject::Render(HDC hdc)
{
}

void GameObject::MakeSnapShot(void* out)
{
}

GameObject::GameObject()
	:Pos({}), bDead(false), ObjectCollider(nullptr), ObjectRigidBody(nullptr)
{
}

GameObject::~GameObject()
{
}
