#include "TestObject.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"

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

	float dt = TimerManager::GetInstance()->GetDeltaTime();
	elapsedTime += dt;

	if (SnapShotManager::GetInstance()->GetPlayer().empty()) return;
	GameObject* player = SnapShotManager::GetInstance()->GetPlayer().front();
	FPOINT pos = player->GetPos();

	float dx = fabs(pos.x - this->GetPos().x);
	float dy = fabs(pos.y - this->GetPos().y);
	//if (dx <= detectionRange && dy <= verticalRange)
	//{
	//	eState = EnemyState::Chasing;
	//	elapsedTime = 0.0f;
	//}
	//switch (eState)
	//{
	//case EnemyState::IDLE:
	//{
	//	if (elapsedTime >= idleDuration)
	//	{
	//		eState = EnemyState::Patrol;
	//		elapsedTime = 0.0f;
	//		dir *= -1; // 순찰 방향 바꾸기
	//	}
	//	
	//}
	//break;
	//case EnemyState::Patrol:
	//{
	//	Pos.x += dir * speed * dt;
	//	if (elapsedTime >= patrolDuration)
	//	{
	//		eState = EnemyState::IDLE;
	//		elapsedTime = 0.0f;
	//	}
	//}
	//break;
	//case EnemyState::Chasing:
	//{
	//	if (SnapShotManager::GetInstance()->GetPlayer().empty())
	//	{
	//		eState = EnemyState::IDLE;
	//		elapsedTime = 0.0f;
	//		break;
	//	}

	//	GameObject* player = SnapShotManager::GetInstance()->GetPlayer().front();
	//	FPOINT playerPos = player->GetPos();

	//	float dx = playerPos.x - Pos.x;
	//	float dy = playerPos.y - Pos.y;

	//	float absDx = fabs(dx);
	//	float absDy = fabs(dy);

	//	// 플레이어가 탐지 범위를 벗어났다면 Idle로 전환
	//	if (absDx > detectionRange || absDy > verticalRange)
	//	{
	//		eState = EnemyState::IDLE;
	//		elapsedTime = 0.0f;
	//		break;
	//	}

	//	// 플레이어 방향으로 이동
	//	float angle = atan2f(0.0f, dx);
	//	Pos.x += cosf(angle) * chasingSpeed * dt;
	//}
	//break;
	//case EnemyState::Attack:
	//{

	//}
	//break;
	//case EnemyState::Dead:
	//{

	//}
	//break;
	//}
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

void TestObject::Patrol()
{

}



void TestObject::Release()

{
}