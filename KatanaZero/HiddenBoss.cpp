#include "HiddenBoss.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "ScrollManager.h"
#include "NormalParts.h"
#include "ArmAttack.h"
#include "Portal.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "BossWeak.h"
#include "LineManager.h"

uniform_int_distribution<int> StateRand(1, 4);

HiddenBoss::HiddenBoss()
	:Face(nullptr), Arm(nullptr), Body(nullptr), ChillGuy(nullptr), State(EState::Idle), CurrentTime(0.f), armAttack(nullptr), NextPos({}), MaxPortalCount(0), PortalCount(0)
	, bossWeak(nullptr), Index(0), Hp(3), CurrentHitTime(0.f), FaceHitTime(3.f)
{
}

HRESULT HiddenBoss::Init()
{
	//Face.image = ImageManager::GetInstance()->FindImage("spr_psychboss_giant_face_idle");
	//Arm.image = ImageManager::GetInstance()->FindImage("spr_psychboss_giant_tentacle_idle");
	//Body.image = ImageManager::GetInstance()->FindImage("spr_psychboss_giant");
	//ChillGuy.image = ImageManager::GetInstance()->FindImage("spr_psychboss_giant_lilguy");

	Face = new NormalParts();
	static_cast<NormalParts*>(Face)->Init("spr_psychboss_giant_face_idle", { Pos.x * 0.85f, Pos.y * 0.825f }, ERenderGroup::BackGround);

	Arm = new NormalParts();
	static_cast<NormalParts*>(Arm)->Init("spr_psychboss_giant_tentacle_idle", { Pos.x * 0.4f, Pos.y * 0.7f }, ERenderGroup::BackGround);

	Body = new NormalParts();
	static_cast<NormalParts*>(Body)->Init("spr_psychboss_giant", { Pos.x * 1.220f, Pos.y * 0.85f }, ERenderGroup::BackGround);
	ChillGuy = new NormalParts();
	static_cast<NormalParts*>(ChillGuy)->Init("spr_psychboss_giant_lilguy", { Pos.x * 1.6f, Pos.y * 1.05f }, ERenderGroup::NonAlphaBlend);

	MaxPortalCount = 8;

	NextPos.x = (WINSIZE_X / 2) / MaxPortalCount;
	NextPos.y = WINSIZE_Y * 0.1f;


	return S_OK;
}

void HiddenBoss::Update()
{
	switch (State)
	{
	case EState::Idle:
		Idle();
		break;
	case EState::ArmAttack:
		ArmAttackState();
		break;
	case EState::PortalAttack1:
		PortalAttack1();
		break;
	case EState::PortalAttack2:
		PortalAttack2();
		break;
	case EState::Weak:
		Weak();
		break;
	case EState::End:
		break;
	default:
		break;
	}

	if (bossWeak != nullptr)
	{
		if (bossWeak->IsHit())
		{
			bossWeak = nullptr;
			if (Face->GetImageKey() != "spr_psychboss_giant_face_hurt")
				Face->ChangeImage("spr_psychboss_giant_face_hurt", false);
			--Hp;
		}
	}

	if (Face->GetImageKey() == "spr_psychboss_giant_face_hurt")
	{
		CurrentHitTime += TimerManager::GetInstance()->GetDeltaTime();
		if (CurrentHitTime >= FaceHitTime)
		{
			CurrentHitTime = 0.f;
			Face->ChangeImage("spr_psychboss_giant_face_idle", false);
		}
	}


	Face->Update();
	Arm->Update();
	Body->Update();
	ChillGuy->Update();
}

void HiddenBoss::Render(HDC hdc)
{/*
	Face.image->FrameRender(hdc, Pos.x, Pos.y, Face.FrameX, 0, false, true, ScrollManager::GetInstance()->GetScale());
	Arm.image->FrameRender(hdc, Pos.x, Pos.y, Arm.FrameX, 0, false, true, ScrollManager::GetInstance()->GetScale());

	Body.image->FrameRender(hdc, Pos.x * 1.25f, Pos.y * 0.7f, Body.FrameX, 0, false, true, ScrollManager::GetInstance()->GetScale());
	ChillGuy.image->FrameRender(hdc, Pos.x * 1.5f, Pos.y, ChillGuy.FrameX, 0, false, true, ScrollManager::GetInstance()->GetScale());*/
}

void HiddenBoss::Idle()
{
	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 2.f)
	{
		CurrentTime = 0.f;
		PreState = State;

		State = (EState)StateRand(dre);
	}
}

void HiddenBoss::ArmAttackState()
{
	if (State != PreState)
	{
		PreState = State;
		Arm->ChangeImage("spr_psychboss_giant_tentacle_stab", true);
	}

	if (Arm->GetImageKey() == "spr_psychboss_giant_tentacle_stab" && Arm->GetFrameX() == Arm->GetMaxFrameX() - 1 && armAttack == nullptr)
	{
		armAttack = new ArmAttack();
		armAttack->Init();
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, armAttack);
	}

	if (armAttack != nullptr && armAttack->IsFinish())
	{
		armAttack->SetDead(true);
		armAttack = nullptr;
		Arm->ChangeImage("spr_psychboss_giant_tentacle_stab_end", true);
	}

	if (Arm->GetImageKey() == "spr_psychboss_giant_tentacle_stab_end" && Arm->GetFrameX() == Arm->GetMaxFrameX() - 1)
	{
		Arm->ChangeImage("spr_psychboss_giant_tentacle_idle", false);
		State = EState::Idle;
	}
}

void HiddenBoss::PortalAttack1()
{
	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 0.25f && PortalCount < MaxPortalCount)
	{
		CurrentTime = 0.f;
		++PortalCount;
		Portal* portal = new Portal();

		portal->Init(NextPos, 90.f, 1);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, portal);

		NextPos.x += 200.f;

		if (PortalCount == MaxPortalCount)
		{
			PortalCount = 0;
			State = EState::Idle;
		}
	}
}

void HiddenBoss::PortalAttack2()
{
	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 0.25f && PortalCount < MaxPortalCount)
	{
		CurrentTime = 0.f;
		++PortalCount;

		std::uniform_int_distribution<int> distX(200, WINSIZE_X - 200);
		std::uniform_int_distribution<int> distY(100, WINSIZE_Y - 100);
		std::uniform_int_distribution<int> dist(0, 1);

		int x = distX(dre);
		int y = distY(dre);

		bool IsX = dist(dre);
		if (IsX)
			y = 100;
		else
		{
			bool Zero = dist(dre);
			x = Zero ? 200 : WINSIZE_X - 200;
		}
		Collider* Target = CollisionManager::GetInstance()->GetTargetCollider(ECollisionGroup::Player);

		if (Target != nullptr)
		{
			float newAngle = RAD_TO_DEG(atan2f(Target->GetPos().y - (float)y, Target->GetPos().x - (float)x));
			Portal* portal = new Portal();

			portal->Init({ (float)x,(float)y }, newAngle, 0);
			ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, portal);
		}

		if (PortalCount == MaxPortalCount)
		{
			PortalCount = 0;
			State = EState::Idle;
		}

	}
}

void HiddenBoss::Weak()
{
	if (PreState == State)
		return;

	Line* line = LineManager::GetInstance()->GetLineFront(ELineType::Normal);
	PreState = State;

	if (bossWeak == nullptr)
	{
		++Index;

		bossWeak = new BossWeak();
		if (line != nullptr)
			bossWeak->Init({ WINSIZE_X * 0.5f,line->start.y + 100.f }, line->start);

		Collider* Target = CollisionManager::GetInstance()->GetTargetCollider(ECollisionGroup::Player);

		Portal* portal = new Portal();
		portal->Init({ Target->GetOwner()->GetPos().x, (float)line->start.y }, 270.f, -1);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, portal);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, bossWeak);

	}

	State = EState::Idle;
}

void HiddenBoss::Release()
{
	if (Face != nullptr)
	{
		Face->Release();
		delete Face;
	}


	if (Arm != nullptr)
	{
		Arm->Release();
		delete Arm;
	}

	if (Body != nullptr)
	{
		Body->Release();
		delete Body;
	}

	if (ChillGuy != nullptr)
	{
		ChillGuy->Release();
		delete ChillGuy;
	}
}
