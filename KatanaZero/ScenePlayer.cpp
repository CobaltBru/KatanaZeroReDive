#include "ScenePlayer.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "Image.h"
#include "ScrollManager.h"

ScenePlayer::ScenePlayer()
	:Speed(0.f)
{
}

HRESULT ScenePlayer::Init()
{
	
	currentState = PSTATE::P_IDLE;

	Speed = 400.f;
	noMove = false;



	float frameSpeed = 0.1f;
	aniIdx = 6;
	float scale = ScrollManager::GetInstance()->GetScale();
	Image* tmp;
	//앉아있음
	tmp = ImageManager::GetInstance()->AddImage("talk_player_sit", L"Image/TalkScene/Player/spr_sitting.bmp", 32, 32, 1, 1, true, RGB(255, 0, 255));
	ani[0].Init(tmp, 1,2.5f);
	ani[0].setPos(Pos, false, true);
	ani[0].setAniTask({
		{0,10.f}
		});
	//idle
	tmp = ImageManager::GetInstance()->AddImage("talk_player_idle", L"Image/TalkScene/Player/spr_casual_idle.bmp", 330, 35, 11, 1, true, RGB(255, 0, 255));
	ani[1].Init(tmp, 11, 2.5f);
	ani[1].setPos(Pos, false, true);
	ani[1].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed}
		});

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { (float)tmp->GetFrameWidth() * 2.5f,
		(float)tmp->GetFrameHeight() * 2.5f }, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	InitRigidBodySetting();
	//걷기
	tmp = ImageManager::GetInstance()->AddImage("talk_player_walk", L"Image/TalkScene/Player/spr_casualwalk.bmp", 272, 36, 8, 1, true, RGB(255, 0, 255));
	ani[2].Init(tmp, 8, 2.5f);
	ani[2].setPos(Pos, false, true);
	ani[2].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed}
		});
	//주사
	tmp = ImageManager::GetInstance()->AddImage("talk_player_getch", L"Image/TalkScene/Player/spr_player_get_chronos.bmp", 210, 32, 7, 1, true, RGB(255, 0, 255));
	ani[3].Init(tmp, 7, 2.5f);
	ani[3].setPos(Pos, false, true);
	ani[3].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed*20.f}
		});
	ani[3].setloopFlag(false);
	//주사복귀
	tmp = ImageManager::GetInstance()->AddImage("talk_player_getch_rt", L"Image/TalkScene/Player/spr_player_get_chronos_return.bmp", 210, 32, 7, 1, true, RGB(255, 0, 255));
	ani[4].Init(tmp, 7, 2.5f);
	ani[4].setPos(Pos, false, true);
	ani[4].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed}
		});
	ani[4].setloopFlag(false);
	//앉기
	tmp = ImageManager::GetInstance()->AddImage("talk_player_sitdown", L"Image/TalkScene/Player/spr_sitdown.bmp", 384, 36, 12, 1, true, RGB(255, 0, 255));
	ani[5].Init(tmp, 12, 2.5f);
	ani[5].setPos({ Pos.x,Pos.y-5.f }, false, true);
	ani[5].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed},{11,frameSpeed}
		});
	ani[5].setloopFlag(false);

	//일어나기
	tmp = ImageManager::GetInstance()->AddImage("talk_player_sitdown2", L"Image/TalkScene/Player/spr_sitdown.bmp", 384, 36, 12, 1, true, RGB(255, 0, 255));
	ani[6].Init(tmp, 12, 2.5f);
	ani[6].setPos(Pos, false, true);
	ani[6].setAniTask({
		{11,frameSpeed},{10,frameSpeed},{9,frameSpeed},
		{8,frameSpeed},{7,frameSpeed},{6,frameSpeed},
		{5,frameSpeed},{4,frameSpeed},{3,frameSpeed},
		{2,frameSpeed},{1,frameSpeed},{0,frameSpeed}
		});
	ani[6].setloopFlag(false);
	ani[currentState].Start();
	return S_OK;
}

void ScenePlayer::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
	for (int i = 0; i < 7; i++)
	{
		ani[i].Release();
	}
}

void ScenePlayer::Update()
{
	
	if (!noMove)
	{
		Move();
		ani[currentState].setPos(Pos, flip, true);
	}
	/*if (KeyManager::GetInstance()->IsStayKeyDown(VK_RETURN))
	{
		setSit();
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('L'))
	{
		if(currentState == PSTATE::P_SIT)
			setUp();
	}*/
	if (KeyManager::GetInstance()->IsStayKeyDown('K'))
	{
		getChronos();
	}
	sitAnima();
	UpAnima();
	ChAnima();
		
	ani[currentState].Update();
	ObjectRigidBody->Update();
	
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void ScenePlayer::Render(HDC hdc)
{
	ani[currentState].Render(hdc);
}

void ScenePlayer::Move()
{
	ObjectRigidBody->SetVelocity({ 0.f,ObjectRigidBody->GetVelocity().y });

	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		flip = true;
		if (currentState != PSTATE::P_WALK)
		{
			ani[currentState].Off();
			currentState = PSTATE::P_WALK;
			ani[currentState].Start();
			ani[currentState].On();
		}
		ObjectRigidBody->AddVelocity({ -Speed,0.f });
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		flip = false;
		if (currentState != PSTATE::P_WALK)
		{
			ani[currentState].Off();
			currentState = PSTATE::P_WALK;
			ani[currentState].Start();
			ani[currentState].On();
		}
		
		ObjectRigidBody->AddVelocity({ Speed,0.f });
	}
	if ((currentState != noMove) && (abs(ObjectRigidBody->GetVelocity().x) <= 0.f))
	{
		if (currentState != PSTATE::P_IDLE)
		{
			ani[currentState].Off();
			currentState = PSTATE::P_IDLE;
			ani[currentState].Start();
			ani[currentState].On();
		}
			
	}

}



void ScenePlayer::InitRigidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// 탄성 적용안함  0 ~ 1 사이
	ObjectRigidBody->SetElasticity(0.3f);

	// 중력 적용
	ObjectRigidBody->SetGravityVisible(true);
	// 저항 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//무게
	ObjectRigidBody->SetMass(5.f);
	//최대 속도
	ObjectRigidBody->SetMaxVelocity({ 120.f,100 });
	//마찰
	ObjectRigidBody->SetFriction(50.f);
}

void ScenePlayer::setSit()
{
	ani[currentState].Off();
	currentState = PSTATE::P_DOWN;
	ani[currentState].setPos(Pos, false, true);
	ani[currentState].Start();
	ani[currentState].On();
	noMove = true;
}

void ScenePlayer::sitAnima()
{
	if (currentState == PSTATE::P_DOWN && (ani[currentState].getAniComplete()))
	{
		ani[currentState].Off();
		currentState = PSTATE::P_SIT;
		ani[currentState].setPos(Pos, false, true);
		ani[currentState].Start();
		ani[currentState].On();
	}
}

void ScenePlayer::setUp()
{
	ani[currentState].Off();
	currentState = PSTATE::P_STANDUP;
	ani[currentState].setPos(Pos, false, true);
	ani[currentState].Start();
	ani[currentState].On();
	
}

void ScenePlayer::UpAnima()
{
	if (currentState == PSTATE::P_STANDUP)
	{
		if (ani[currentState].getAniComplete())
		{
			ani[currentState].Off();
			currentState = PSTATE::P_IDLE;
			ani[currentState].setPos(Pos, false, true);
			ani[currentState].Start();
			ani[currentState].On();
			noMove = false;
		}
		
	}
}

void ScenePlayer::getChronos()
{
	ani[currentState].Off();
	currentState = PSTATE::P_CH;
	ani[currentState].setPos(Pos, false, true);
	ani[currentState].Start();
	ani[currentState].On();
}

void ScenePlayer::ChAnima()
{
	if (currentState == PSTATE::P_CH && (ani[currentState].getAniComplete()))
	{
		ani[currentState].Off();
		currentState = PSTATE::P_CHRT;
		ani[currentState].setPos(Pos, false, true);
		ani[currentState].Start();
		ani[currentState].On();
	}
	else if (currentState == PSTATE::P_CHRT && (ani[currentState].getAniComplete()))
	{
		ani[currentState].Off();
		currentState = PSTATE::P_SIT;
		ani[currentState].setPos(Pos, false, true);
		ani[currentState].Start();
		ani[currentState].On();
	}
}

void ScenePlayer::sitUP()
{
	if (noMove)
	{
		setUp();
	}
	else
	{
		setSit();
	}
}


