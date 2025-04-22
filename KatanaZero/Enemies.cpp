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
	detectRange = 50.f;
	attackRange = 40.f;
	attackDuration = 0.8f;
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
	eState = new EIDLE();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 70.f;
	detectRange = 80.f;
	attackRange = 40.f;
	attackDuration = 0.5f;
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
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/Pomp/Pomp_IDLE.png", 8, 1);
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/Pomp/Pomp_Walk.png", 10, 1);
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/Pomp/Pomp_Run.png", 10, 1);
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/Pomp/Pomp_Attack.png", 6, 1);
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/Pomp/Pomp_Dead.png", 15, 1);
	image = images[(int)EImageType::IDLE];
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
	detectRange = 200.f;
	attackRange = 150.f;
	attackDuration = 0.2f;
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
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/Gangster/Gangster_IDLE.png", 8, 1);
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/Gangster/Gangster_Walk.png", 8, 1);
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/Gangster/Gangster_Run.png", 10, 1);
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/Gangster/Gangster_MeleeAttack.png", 6, 1);
	images[(int)EImageType::GangsterAttack]->AddImage(L"Image/Enemy/Gangster/Gangster_Gun.png", 7, 1);
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/Gangster/Gangster_Dead.png", 14, 1);
	image = images[(int)EImageType::IDLE];
}

HRESULT ShieldCop::Init(FPOINT InPos)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 20.f;
	detectRange = 50.f;
	attackDuration = 1.f;
	eType = EType::ShieldCop;
	InitRigidBodySetting();
	return S_OK;
}

void ShieldCop::InitImages()
{
	images.resize((int)EImageType::End);
	for (int i = 0; i < images.size(); i++)
	{
		images[i] = new GPImage();
	}
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_IDLE.png", 6, 1);
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Walk.png", 10, 1);
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Run.png", 10, 1);
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Bash.png", 6, 1);
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/ShieldCop/ShieldCop_Dead.png", 15, 1);
	image = images[(int)EImageType::IDLE];
}
