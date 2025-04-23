#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"

enum PSTATE { P_SIT, P_IDLE, P_WALK, P_CH, P_CHRT, P_DOWN, P_STANDUP };

class ScenePlayer : public GameObject
{
private:
	float Speed;

	Animation ani[7];
	int aniIdx;
	PSTATE currentState;
	bool flip;
	bool noMove;
public:
	ScenePlayer();
	virtual ~ScenePlayer() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Move();
	void InitRigidBodySetting();
	void setSit();
	void sitAnima();
	void setUp();
	void UpAnima();
	void getChronos();
	void ChAnima();

	void sitUP();
};

