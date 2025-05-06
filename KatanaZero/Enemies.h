#pragma once
#include "Enemy.h"
#include "config.h"

class Grunt : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup) override;
	virtual void InitImages() override;
	virtual void SetAnimKey(EImageType newImage) override;

	
};

class Pomp : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup) override;
	virtual void InitImages() override;
	virtual void SetAnimKey(EImageType newImage) override;
};

class Gangster : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup) override;
	virtual void InitImages() override;
	virtual void SetAnimKey(EImageType newImage) override;
};
