#include "GameObject.h"

HRESULT GameObject::Init()
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

int GameObject::GetFloorIndex() const
{
	float y = this->GetPos().y;
	for (int i = 0; i < FloorZones.size(); i++)
	{
		if (FloorZones[i].Contains(y))
		{
			return i;
		}
	}

	return -1;
}

GameObject::GameObject()
	:Pos({}), bDead(false), ObjectCollider(nullptr), ObjectRigidBody(nullptr)
{
}

GameObject::~GameObject()
{
}
