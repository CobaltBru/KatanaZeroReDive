#include "GameObject.h"


HRESULT GameObject::Init()
{
	return S_OK;
}

HRESULT GameObject::Init(FPOINT Pos)
{
	return S_OK;
}

HRESULT GameObject::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
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

int GameObject::GetFloorIndex(vector<FloorZone> fz) const
{
	float y = this->GetPos().y;
	for (int i = 0; i < fz.size(); i++)
	{
		if (fz[i].Contains(y))
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
