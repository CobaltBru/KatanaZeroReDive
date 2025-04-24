#include "Enemies.h"
#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"
#include "RenderManager.h"
#include "EnemyState.h"
#include "ScrollManager.h"
#include "GPImageManager.h"

HRESULT Grunt::Init(FPOINT InPos)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 50.f;
	detectRange = 400.f;
	attackRange = 50.f;
	attackDuration = 0.8f;
	eType = EType::Grunt;
	InitRigidBodySetting();
	return S_OK;
}

HRESULT Grunt::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	Speed = 50.f;
	detectRange = 400.f;
	attackRange = 50.f;
	attackDuration = 0.8f;
	eType = EType::Grunt;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);
	
	InitRigidBodySetting();
	return S_OK;
}

void Grunt::InitImages()
{
	images.resize((int)EImageType::End);
	GPImageManager::GetInstance()->AddImage("Grunt_IDLE", L"Image/Enemy/Grunt/Grunt_IDLE.png", 8, 1);
	GPImageManager::GetInstance()->AddImage("Grunt_Walk", L"Image/Enemy/Grunt/Grunt_Walk.png", 10, 1);
	GPImageManager::GetInstance()->AddImage("Grunt_Run", L"Image/Enemy/Grunt/Grunt_Run.png", 10, 1);
	GPImageManager::GetInstance()->AddImage("Grunt_Attack", L"Image/Enemy/Grunt/Grunt_Attack.png", 8, 1);
	GPImageManager::GetInstance()->AddImage("Grunt_Dead", L"Image/Enemy/Grunt/Grunt_Dead.png", 16, 1);
	images[(int)EImageType::IDLE] = GPImageManager::GetInstance()->FindImage("Grunt_IDLE");
	images[(int)EImageType::Walk] = GPImageManager::GetInstance()->FindImage("Grunt_Walk");
	images[(int)EImageType::Run] = GPImageManager::GetInstance()->FindImage("Grunt_Run");
	images[(int)EImageType::Attack] = GPImageManager::GetInstance()->FindImage("Grunt_Attack");
	images[(int)EImageType::Dead] = GPImageManager::GetInstance()->FindImage("Grunt_Dead");
	image = images[(int)EImageType::IDLE];
	SetAnimKey(EImageType::IDLE);
	int a = 1;
}

void Grunt::SetAnimKey(EImageType newImage)
{
	switch (newImage)
	{
	case EImageType::IDLE:
		currAnimKey = "Grunt_IDLE";
		break;
	case EImageType::Walk:
		currAnimKey = "Grunt_Walk";
		break;
	case EImageType::Run:
		currAnimKey = "Grunt_Run";
		break;
	case EImageType::Attack:
		currAnimKey = "Grunt_Attack";
		break;
	case EImageType::Dead:
		currAnimKey = "Grunt_Dead";
		break;
	}
}

HRESULT Pomp::Init(FPOINT InPos)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 70.f;
	detectRange = 600.f;
	attackRange = 60.f;
	attackDuration = 0.5f;
	eType = EType::Pomp;
	InitRigidBodySetting();
	return S_OK;
}

HRESULT Pomp::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	Speed = 70.f;
	detectRange = 600.f;
	attackRange = 60.f;
	attackDuration = 0.5f;
	eType = EType::Pomp;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);

	InitRigidBodySetting();
	return S_OK;
}

void Pomp::InitImages()
{
	images.resize((int)EImageType::End);
	GPImageManager::GetInstance()->AddImage("Pomp_IDLE", L"Image/Enemy/Pomp/Pomp_IDLE.png", 8, 1);
	GPImageManager::GetInstance()->AddImage("Pomp_Walk", L"Image/Enemy/Pomp/Pomp_Walk.png", 10, 1);
	GPImageManager::GetInstance()->AddImage("Pomp_Run", L"Image/Enemy/Pomp/Pomp_Run.png", 10, 1);
	GPImageManager::GetInstance()->AddImage("Pomp_Attack", L"Image/Enemy/Pomp/Pomp_Attack.png", 6, 1);
	GPImageManager::GetInstance()->AddImage("Pomp_Dead", L"Image/Enemy/Pomp/Pomp_Dead.png", 15, 1);
	images[(int)EImageType::IDLE] = GPImageManager::GetInstance()->FindImage("Pomp_IDLE");
	images[(int)EImageType::Walk] = GPImageManager::GetInstance()->FindImage("Pomp_Walk");
	images[(int)EImageType::Run] = GPImageManager::GetInstance()->FindImage("Pomp_Run");
	images[(int)EImageType::Attack] = GPImageManager::GetInstance()->FindImage("Pomp_Attack");
	images[(int)EImageType::Dead] = GPImageManager::GetInstance()->FindImage("Pomp_Dead");
	image = images[(int)EImageType::IDLE];
	SetAnimKey(EImageType::IDLE);
}

void Pomp::SetAnimKey(EImageType newImage)
{
	switch (newImage)
	{
	case EImageType::IDLE:
		currAnimKey = "Pomp_IDLE";
		break;
	case EImageType::Walk:
		currAnimKey = "Pomp_Walk";
		break;
	case EImageType::Run:
		currAnimKey = "Pomp_Run";
		break;
	case EImageType::Attack:
		currAnimKey = "Pomp_Attack";
		break;
	case EImageType::Dead:
		currAnimKey = "Pomp_Dead";
		break;
	}
}

HRESULT Gangster::Init(FPOINT InPos)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 40.f;
	detectRange = 500.f;
	attackRange = 400.f;
	attackDuration = 0.2f;
	eType = EType::Gangster;
	InitRigidBodySetting();
	return S_OK;
}

HRESULT Gangster::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	Speed = 40.f;
	detectRange = 500.f;
	attackRange = 400.f;
	attackDuration = 0.2f;
	eType = EType::Gangster;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);

	InitRigidBodySetting();
	return S_OK;
}

void Gangster::InitImages()
{
	images.resize((int)EImageType::End);
	GPImageManager::GetInstance()->AddImage("Gangster_IDLE", L"Image/Enemy/Gangster/Gangster_IDLE.png", 8, 1);
	GPImageManager::GetInstance()->AddImage("Gangster_Walk", L"Image/Enemy/Gangster/Gangster_Walk.png", 8, 1);
	GPImageManager::GetInstance()->AddImage("Gangster_Run", L"Image/Enemy/Gangster/Gangster_Run.png", 10, 1);
	GPImageManager::GetInstance()->AddImage("Gangster_MeleeAttack", L"Image/Enemy/Gangster/Gangster_MeleeAttack.png", 6, 1);
	GPImageManager::GetInstance()->AddImage("Gangster_Gun", L"Image/Enemy/Gangster/Gangster_Gun.png", 7, 1);
	GPImageManager::GetInstance()->AddImage("Gangster_Dead", L"Image/Enemy/Gangster/Gangster_Dead.png", 14, 1);
	images[(int)EImageType::IDLE] = GPImageManager::GetInstance()->FindImage("Gangster_IDLE");
	images[(int)EImageType::Walk] = GPImageManager::GetInstance()->FindImage("Gangster_Walk");
	images[(int)EImageType::Run] = GPImageManager::GetInstance()->FindImage("Gangster_Run");
	images[(int)EImageType::Attack] = GPImageManager::GetInstance()->FindImage("Gangster_MeleeAttack");
	images[(int)EImageType::GangsterAttack] = GPImageManager::GetInstance()->FindImage("Gangster_Gun");
	images[(int)EImageType::Dead] = GPImageManager::GetInstance()->FindImage("Gangster_Dead");
	image = images[(int)EImageType::IDLE];
	SetAnimKey(EImageType::IDLE);
}

void Gangster::SetAnimKey(EImageType newImage)
{
	switch (newImage)
	{
	case EImageType::IDLE:
		currAnimKey = "Gangster_IDLE";
		break;
	case EImageType::Walk:
		currAnimKey = "Gangster_Walk";
		break;
	case EImageType::Run:
		currAnimKey = "Gangster_Run";
		break;
	case EImageType::Attack:
		currAnimKey = "Gangster_MeleeAttack";
		break;
	case EImageType::GangsterAttack:
		currAnimKey = "Gangster_Gun";
		break;
	case EImageType::Dead:
		currAnimKey = "Gangster_Dead";
		break;
	}
}


