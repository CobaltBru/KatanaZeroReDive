#pragma once
#include "config.h"


class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual HRESULT Init();		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);

	FPOINT GetPos() const { return Pos; }

	FPOINT GetSpawnPos() { return spawnPos; }
	void SetSpawnPos(FPOINT spawnPos) { this->spawnPos = spawnPos; }

protected:
	FPOINT Pos;

	FPOINT spawnPos;
};

