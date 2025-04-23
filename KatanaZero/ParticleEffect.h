#pragma once
#include "GameObject.h"

class Particle : GameObject
{

private:
	float scale;

public:
	Particle();
	virtual ~Particle() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;


	void InitRigidBodySetting();

};

class ParticleEffect 
{

};

