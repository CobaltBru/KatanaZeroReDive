#include "Enemies.h"
#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"
#include "RenderManager.h"
#include "EnemyState.h"

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
	detectRange = 100.f;
	eType = EType::Grunt;
	InitRigidBodySetting();
	return S_OK;
}

void Grunt::InitImages()
{
	images.resize((int)EImageType::End);
	for (int i = 0; i < images.size(); i++)
	{
		images[i] = new GPImage();
	}
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/Grunt/Grunt_IDLE.png", 8, 1);
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/Grunt/Grunt_Walk.png", 10, 1);
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/Grunt/Grunt_Run.png", 10, 1);
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/Grunt/Grunt_Attack.png", 8, 1);
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/Grunt/Grunt_Dead.png", 16, 1);
	image = images[(int)EImageType::IDLE];
}

HRESULT Pomp::Init(FPOINT InPos)
{
	InitImages();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 200.f;
	eType = EType::Pomp;
	InitRigidBodySetting();
	return S_OK;
}

void Pomp::InitImages()
{
	images.resize((int)EImageType::End);
	for (int i = 0; i < images.size(); i++)
	{
		images[i] = new GPImage();
	}
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/Pomp/Pomp_IDLE.png");
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/Pomp/Pomp_Walk.png");
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/Pomp/Pomp_Run.png");
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/Pomp/Pomp_Attack.png");
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/Pomp/Pomp_Dead.png");
	image = images[(int)EImageType::IDLE];
}

HRESULT Gangster::Init(FPOINT InPos)
{
	InitImages();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 200.f;
	eType = EType::Gangster;
	InitRigidBodySetting();
	return S_OK;
}

void Gangster::InitImages()
{
	images.resize((int)EImageType::End);
	for (int i = 0; i < images.size(); i++)
	{
		images[i] = new GPImage();
	}
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/Gangster/Gangster_IDLE.png");
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/Gangster/Gangster_Walk.png");
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/Gangster/Gangster_Run.png");
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/Gangster/Gangster_MeleeAttack.png");
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/Gangster/Gangster_Dead.png");
	image = images[(int)EImageType::IDLE];
}

HRESULT SheildCop::Init(FPOINT InPos)
{
	InitImages();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 200.f;
	eType = EType::SheildCop;
	InitRigidBodySetting();
	return S_OK;
}

void SheildCop::InitImages()
{
	images.resize((int)EImageType::End);
	for (int i = 0; i < images.size(); i++)
	{
		images[i] = new GPImage();
	}
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_IDLE.png");
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Walk.png");
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Run.png");
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Bash.png");
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Dead.png");
	image = images[(int)EImageType::IDLE];
}
