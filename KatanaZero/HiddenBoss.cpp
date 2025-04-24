#include "HiddenBoss.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "ScrollManager.h"
#include "NormalParts.h"
#include "ArmAttack.h"

HiddenBoss::HiddenBoss()
	:Face(nullptr), Arm(nullptr), Body(nullptr), ChillGuy(nullptr), State(EState::Idle), CurrentTime(0.f), armAttack(nullptr)
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
	static_cast<NormalParts*>(Body)->Init("spr_psychboss_giant",{ Pos.x * 1.220f, Pos.y * 0.85f },ERenderGroup::BackGround);
	ChillGuy = new NormalParts();
	static_cast<NormalParts*>(ChillGuy)->Init("spr_psychboss_giant_lilguy", { Pos.x * 1.6f, Pos.y * 1.05f }, ERenderGroup::NonAlphaBlend);

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
	case EState::End:
		break;
	default:
		break;
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

void HiddenBoss::Idle()
{
	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 1.f)
	{
		CurrentTime = 0.f;
		PreState = State;
		State = EState::ArmAttack;
	}
}

void HiddenBoss::ArmAttackState()
{
	if (State != PreState)
	{
		PreState = State;
		Arm->ChangeImage("spr_psychboss_giant_tentacle_stab",true);
	}

	if (Arm->GetImageKey() == "spr_psychboss_giant_tentacle_stab" && Arm->GetFrameX() == Arm->GetMaxFrameX() - 1 && armAttack == nullptr)
	{
		armAttack = new ArmAttack();
		armAttack->Init();
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, armAttack);
	}
}
