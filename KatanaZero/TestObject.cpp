#include "TestObject.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "Collider.h"
#include "CollisionManager.h"

TestObject::TestObject()
	:Image(nullptr), ObjectCollider(nullptr)
{
}

void TestObject::Init(string InImageKey, FPOINT InPos)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);

	ObjectCollider = new Collider(this,EColliderType::Rect,{},30.f,true,1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);

	Pos = InPos;	
}

void TestObject::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void TestObject::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		// 스크롤이 필요한 오브젝트들
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y,0,0);
	}
}

void TestObject::MakeSnapShot(void* out)
{
	EnemySnapShot* eSnapShot = static_cast<EnemySnapShot*>(out);
	eSnapShot->pos = this->GetPos();
	eSnapShot->ID = 0;
	eSnapShot->animFrame = 0;
	eSnapShot->isDead = false;
}

void TestObject::ApplySnapShot(const EnemySnapShot& eSnapShot)
{
	this->Pos = eSnapShot.pos;
}



void TestObject::Release()

{
}