#include "Grunt.h"
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
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 200.f;

	return S_OK;
}

void Grunt::InitImages()
{
	images.resize((int)EImageType::End);
	images[(int)EImageType::IDLE]->AddImage(L"Image/Enemy/Grunt/Grunt_IDLE.png");
	images[(int)EImageType::Walk]->AddImage(L"Image/Enemy/Grunt/Grunt_Walk.png");
	images[(int)EImageType::Run]->AddImage(L"Image/Enemy/Grunt/Grunt_Run.png");
	images[(int)EImageType::Attack]->AddImage(L"Image/Enemy/Grunt/Grunt_Attack.png");
	images[(int)EImageType::Dead]->AddImage(L"Image/Enemy/Grunt/Grunt_Dead.png");
	image = images[(int)EImageType::IDLE];
}

void Grunt::Update()
{
	UpdateAnimation();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
	if (eState)
		eState->Update(*this);
}

void Grunt::Render(HDC hdc)
{
	if (image)
	{
		Gdiplus::Graphics graphics(hdc);
		image->Middle_RenderFrameScale(&graphics, Pos, currFrame, bFlip, 1.0f, 2.5f, 2.5f);
	}
}
