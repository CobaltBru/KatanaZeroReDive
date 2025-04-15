#include "Character.h"
#include "Collider.h"


HRESULT Character::Init()
{
	CharacterCollider = nullptr;

	return S_OK;
}


void Character::Release()
{
}

void Character::Update()
{
}

void Character::Render(HDC hdc)
{
}

void Character::MakeSnapShot(void* out)
{

}


void Character::Move()
{
	Pos.x++;		
}

void Character::Jump()
{
}

void Character::Attack()
{
}

void Character::Down()
{
}

void Character::WallSlide()
{
}
