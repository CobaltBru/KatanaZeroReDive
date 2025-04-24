#pragma once
#include "GameObject.h"



class Particle : public GameObject
{

private:

	float scale;
	float angle;
	float force;

	float timer;
public:
	Particle();
	virtual ~Particle() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;


	void InitRigidBodySetting();
	inline float getTimer() { return timer; }

	inline float GetLength(const FPOINT& InPoint)
	{
		return sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y);
	}

	inline void Normalize(FPOINT& InPoint)
	{
		const float Length = GetLength(InPoint);
		if (InPoint.x != 0.f)
			InPoint.x /= Length;

		if (InPoint.y != 0.f)
			InPoint.y /= Length;
	}
};

class ParticleEffect : public GameObject
{
private:
	list<Particle*> activeList;
	list<Particle*> freeList;
	float timer;
	float delay;
public:
	ParticleEffect();
	virtual ~ParticleEffect() = default;
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void activeParticle();
	void freeParticle();
};

