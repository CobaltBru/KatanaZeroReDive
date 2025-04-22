#pragma once
#include "Enemy.h"
#include "config.h"

class Grunt : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;
};

class Pomp : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;
};

class Gangster : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;
};

class ShieldCop : public Enemy
{
public:
	virtual HRESULT Init(FPOINT InPos) override;
	virtual void InitImages() override;
};
