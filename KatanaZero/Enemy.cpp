#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"
#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "CommonFunction.h"
#include "PathFinder.h"
#include "Player.h"

Enemy::Enemy()
	:image(nullptr), eState(nullptr), currFrame(0), Speed(0.f), frameTimer(0.f), bFlip(false), bJump(false), dY(-10.f), 
	Gravity(0.1f), bFalling(true), bDown(false), dir(1), detectRange(0.f), attackRange(0.f), eType(EType::None), targetFloor(-1),
	bReachedTargetFloor(false), attackDuration(0.f), gunRange(0.f), currAnimKey(""), HitAngle(0.f), pathFinder(nullptr)
{
}

Enemy::~Enemy()
{
}

HRESULT Enemy::Init(FPOINT InPos)
{
	return E_NOTIMPL;
}

HRESULT Enemy::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	return E_NOTIMPL;
}

void Enemy::InitImages()
{
}

void Enemy::InitRigidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// ź�� �������  0 ~ 1 ����
	ObjectRigidBody->SetElasticity(0.3f);

	// �߷� ����
	ObjectRigidBody->SetGravityVisible(true);
	// ���� 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//����
	ObjectRigidBody->SetMass(15.f);
	//�ִ� �ӵ�
	ObjectRigidBody->SetMaxVelocity({ Speed * 2.f , 4000.f });
	
	//����
	ObjectRigidBody->SetFriction(50.f);

	//������ �������� ���� ��
	//ObjectRigidBody->SetDown(true);
}

void Enemy::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
	if (eState)
	{
		delete eState;
		eState = nullptr;
	}
	image = nullptr;
	for (auto& img : images)
	{
		if (img)
		{
			img = nullptr;
		}
	}
	images.clear();

	if (pathFinder)
	{
		delete pathFinder;
		pathFinder = nullptr;
	}

	if (root)
	{
		delete root;
		root = nullptr;
	}
}

void Enemy::Update()
{
	LastPos = Pos;
	/*switch (eType)
	{
	case EType::Gangster:
		AttackCollider->SetPivot({ meleeAttackRange / 2.f * dir, 0.f });
		break;
	case EType::Enemy:
	case EType::Pomp:
		AttackCollider->SetPivot({ attackRange / 2.f * dir, 0.f });
		break;
	}
	UpdateAnimation();
	if (eState)
	{
		eState->Update(*this);
		EnemyState* newState = eState->CheckTransition(this);
		if (newState && newState != eState)
		{
			ChangeState(newState);
		}
	}*/
	if (!bDead)
	{
		ObjectRigidBody->SetVelocity({ 0.f, 0.f });
		float px = baseAttackPivotX * dir;
		float py = baseAttackPivotY;
		AttackCollider->SetPivot({ px, py });
		float dt = TimerManager::GetInstance()->GetDeltaTime();
		attackTimer -= dt;
		if (attackTimer <= 0.f)
		{
			attackTimer = 0.f;
		}
		if (ObjectCollider->IsHitted() && !bHitted)
		{
			bHitted = true;
		}
		NodeStatus nodeStatus = root->tick();
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Enemy::Render(HDC hdc)
{
	if (image)
	{
		Gdiplus::Graphics graphics(hdc);
		image->Middle_RenderFrameScale(&graphics, { Pos.x + ScrollManager::GetInstance()->GetScroll().x, Pos.y + ScrollManager::GetInstance()->GetScroll().y }, currFrame, bFlip, 1.0f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
	}

	// ��� ������
	auto& nodes = LineManager::GetInstance()->GetNodes();
	HPEN hOld = nullptr;
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  // ������ ����� ��
	hOld = (HPEN)SelectObject(hdc, hPen);

	POINT prev;
	bool first = true;
	for (int idx : debugPath) {
		FPOINT wp = nodes[idx];
		// Scroll ����
		int sx = (int)(wp.x + ScrollManager::GetInstance()->GetScroll().x);
		int sy = (int)(wp.y + ScrollManager::GetInstance()->GetScroll().y);
		if (first) {
			prev = { sx, sy };
			first = false;
		}
		else {
			MoveToEx(hdc, prev.x, prev.y, nullptr);
			LineTo(hdc, sx, sy);
			prev = { sx, sy };
		}
	}

	// ����
	SelectObject(hdc, hOld);
	DeleteObject(hPen);
}

void Enemy::MakeSnapShot(void* out)
{
	EnemySnapShot* eSnapShot = static_cast<EnemySnapShot*>(out);
	eSnapShot->pos = this->GetPos();
	eSnapShot->animKey = currAnimKey;
	eSnapShot->animFrame = currFrame;
	eSnapShot->isDead = this->bDead;
	eSnapShot->bFlip = this->bFlip;
}

int Enemy::GetMaxAttackFrame() const
{
	if (image == nullptr) return 0;
	return images[(int)EImageType::Attack]->getMaxFrame();
}

void Enemy::UpdateAnimation()
{
	if (image == nullptr) return;
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	frameTimer += dt;
	if (frameTimer > 0.1f)
	{
		currFrame++;
		if (currFrame >= image->getMaxFrame())
		{
			currFrame = 0;
		}
		frameTimer = 0.f;
	}
	if (dir == -1)
	{
		bFlip = true;
	}
	else if(dir == 1)
	{
		bFlip = false;
	}
}

void Enemy::ChangeState(EnemyState* newState)
{
	if (eState)
	{
		eState->Exit(*this);
		delete eState;
	}
	eState = newState;
	if (eState)
	{
		eState->Enter(*this);
	}
		
}

void Enemy::ChangeAnimation(EImageType newImage)
{
	if (image == nullptr) return;
	
	if (image == images[(int)newImage]) return;
	
	currFrame = 0;
	image = images[(int)newImage];
	SetAnimKey(newImage);
}

bool Enemy::Detecting()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return false;
	FHitResult Result;
	if (CollisionManager::GetInstance()->LineTraceByObject(Result, ECollisionGroup::Player, this->Pos, { Pos.x + detectRange * dir, Pos.y }))
	{
		return true;
	}
	return false;
}

bool Enemy::IsInAttackRange()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return false;
	FHitResult Result; // �浹 ���
	if (CollisionManager::GetInstance()->LineTraceByObject(Result, ECollisionGroup::Player, this->Pos, { Pos.x + attackRange * dir, Pos.y }))
	{
		return true;
	}
		
	return false;
}

bool Enemy::IsInSameFloor()
{
	auto player = SnapShotManager::GetInstance()->GetPlayer();
	if (!player) return true;

	int myFloor = this->GetFloorIndex(g_FloorZones);
	int playerFloor = player->GetFloorIndex(g_FloorZones);

	return myFloor == playerFloor;
}

bool Enemy::IsOnDownLine()
{
	return GetRigidBody()->GetResult().LineType == ELineType::DownLine;
}

bool Enemy::IsHitted()
{
	if (ObjectCollider->IsHitted()) return true;
	return false;
}

void Enemy::Collision()
{
	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(AttackCollider, HitResult, ECollisionGroup::Player))
	{
		HitResult.HitCollision->SetHit(true);
		auto player = static_cast<Player*>(HitResult.HitCollision->GetOwner());
		FPOINT pos;
		pos.x = HitResult.HitCollision->GetPos().x - AttackCollider->GetPos().x;
		pos.y = HitResult.HitCollision->GetPos().y - AttackCollider->GetPos().y;
		Normalize(pos);
		HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(pos * 500.f);
		if (player->getCurrentState() != STATE::DEAD)
			player->changeState(STATE::DEAD);
	}
}

NodeStatus Enemy::IDLEAction()
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	idleTimer += dt;
	if (idleTimer >= idleDuration)
	{
		canPatrol = true;
		idleTimer = 0.f;
		return NodeStatus::Success;
	}
	ObjectRigidBody->Update();
	UpdateAnimation();

	return NodeStatus::Running;
}

NodeStatus Enemy::PatrolAction()
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	patrolTimer += dt;
	if (patrolTimer >= patrolDuration)
	{
		canPatrol = false;
		patrolTimer = 0.f;
		ObjectRigidBody->SetVelocity({ 0.f, 0.f });
		SetDir(GetDir() * -1);
		return NodeStatus::Success;
	}
	int dir = GetDir();
	const float speed = GetSpeed();
	ObjectRigidBody->AddVelocity({ dir * speed, 0.f });
	ObjectRigidBody->Update();
	UpdateAnimation();
	return NodeStatus::Running;
}

NodeStatus Enemy::DeadAction()
{
	if (GetCurrFrame() >= GetImage()->getMaxFrame() - 1)
	{
		SetDead(true);
		return NodeStatus::Success;
	}
	ObjectRigidBody->Update();
	UpdateAnimation();
	return NodeStatus::Running;
}

NodeStatus Enemy::MeleeAttackAction()
{
	if (GetCurrFrame() >= GetImage()->getMaxFrame() - 1)
	{
		attackTimer = attackDuration;
		bAttacking = false;
		return NodeStatus::Success;
	}
	if (GetCurrFrame() >= 3 && GetCurrFrame() < GetImage()->getMaxFrame() - 1)
	{
		Collision();
	}
	ObjectRigidBody->Update();
	UpdateAnimation();
	return NodeStatus::Running;
}

NodeStatus Enemy::ChaseAction()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return NodeStatus::Failure;
	bChasing = true;
	auto player = SnapShotManager::GetInstance()->GetPlayer();
	FPOINT playerPos = player->GetPos();

	float dx = playerPos.x - Pos.x;
	int dir = (dx > 0) ? 1 : -1;
	SetDir(dir);

	const float chaseSpeed = GetSpeed() * 2.f;
	ObjectRigidBody->AddVelocity({ dir * chaseSpeed, 0.f });
	ObjectRigidBody->Update();
	UpdateAnimation();
	if (IsInAttackRange()) return NodeStatus::Success;
	if (!Detecting()) return NodeStatus::Failure;

	return NodeStatus::Running;
}

NodeStatus Enemy::CalcPathAction()
{
	auto* player = SnapShotManager::GetInstance()->GetPlayer();
	if (!player) return NodeStatus::Failure;

	// A* ��� ���� ���
	int startIdx = pathFinder->findClosestNode(this->Pos);
	int goalIdx = pathFinder->findClosestNode(player->GetPos());
	auto newPath = pathFinder->FindPath(startIdx, goalIdx);
	if (newPath.empty()) {
		navPath.clear();
		return NodeStatus::Failure;
	}

	// ��� ��ġ ����
	const auto& nodes = LineManager::GetInstance()->GetNodes();

	// 1) �����÷��̾� ���� ����
	FPOINT toPlayer{ player->GetPos().x - Pos.x,
					 player->GetPos().y - Pos.y };
	// ����ȭ
	float lenP = sqrtf(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
	if (lenP > 0.f) {
		toPlayer.x /= lenP;
		toPlayer.y /= lenP;
	}

	// 2) ���÷��̾� ���� �ĺ����� ���� �� �ĺ��� �и�
	vector<size_t> forwardCandidates;
	vector<size_t> otherCandidates;
	forwardCandidates.reserve(newPath.size());
	otherCandidates.reserve(newPath.size());

	for (size_t i = 0; i < newPath.size(); ++i) {
		FPOINT nodePos = nodes[newPath[i]];
		// ������ ����
		float dx = nodePos.x - Pos.x;
		float dy = nodePos.y - Pos.y;
		// �÷��̾� ������� ����
		float dot = dx * toPlayer.x + dy * toPlayer.y;
		if (dot > 0.f) {
			forwardCandidates.push_back(i);
		}
		else {
			otherCandidates.push_back(i);
		}
	}

	// 3) �켱����: forwardCandidates, ������ otherCandidates
	auto& pickList = forwardCandidates.empty()
		? otherCandidates
		: forwardCandidates;

	// 4) ���߿��� ���� ���� ����� ��带 ����
	float bestDist = FLT_MAX;
	size_t bestI = pickList.front();
	for (size_t idx : pickList) {
		FPOINT nodePos = nodes[newPath[idx]];
		float dx = nodePos.x - Pos.x;
		float dy = nodePos.y - Pos.y;
		float d = sqrtf(dx * dx + dy * dy);
		if (d < bestDist) {
			bestDist = d;
			bestI = idx;
		}
	}
	if (bestI == 0 && newPath.size() > 1) {
		bestI = 1;  // �ٷ� ���� ��������Ʈ���� ����
	}

	// 5) navPath ����
	navPath.setPath(newPath);
	debugPath = navPath.getFullPath();
	navPath.setCurrentIdx(bestI);

	return NodeStatus::Success;
}

NodeStatus Enemy::FindPathAction()
{
	// 1) ��� ������� ���� ����
	if (navPath.isEmpty()) {
		ObjectRigidBody->SetVelocity({ 0,0 });
		return NodeStatus::Failure;
	}

	// 2) ���硤���� ���
	int u = navPath.getCurrentNode();
	int uNext = navPath.peekNextNode();

	if (uNext != -1) {
		auto& edges = LineManager::GetInstance()->GetGraph()[u];
		for (auto& e : edges) {
			if (e.to != uNext) continue;      // �� ������ ���� ������ �� ������ �ƴ϶�� skip

			bool goingDown =
				LineManager::GetInstance()->GetNodes()[uNext].y
		  > LineManager::GetInstance()->GetNodes()[u].y;

		  if (e.type == ELineType::DownLine && goingDown) {
			  // �������� DownLine ����
			  if (!bDown) {
				  bDown = true;
				  ObjectRigidBody->SetDown(true);
			  }
		  }
		  else {
			  // �� �� (NormalLine �̰� ����, �ö󰡴� DownLine �̰� �׳� �ٴ� �浹)
			  if (bDown) {
				  bDown = false;
				  ObjectRigidBody->SetDown(false);
			  }
		  }
		  break;  // �� �� �˻������� ���� Ż��
		}
	}

	// 4) �̵�
	const auto& nodes = LineManager::GetInstance()->GetNodes();
	FPOINT target = (uNext != -1)
		? nodes[uNext]
		: nodes[u];

		float dx = target.x - Pos.x;
		if (fabsf(dx) < 2.f) {
			// ��������Ʈ ����
			navPath.advance();
			return NodeStatus::Running;
		}

		float dirX = dx > 0.f ? +1.f : -1.f;
		const float chaseSpeed = GetSpeed() * 2.f;
		ObjectRigidBody->AddVelocity({ dirX * chaseSpeed, 0.f });
		SetDir((int)dirX);

		ObjectRigidBody->Update();
		UpdateAnimation();
		return NodeStatus::Running;
}

NodeStatus Enemy::WatingAction()
{
	/*if (GetCurrFrame() >= GetImage()->getMaxFrame() - 1)
	{
		return NodeStatus::Success;
	}*/
	ObjectRigidBody->SetVelocity({ 0.f, 0.f });
	ObjectRigidBody->Update();
	UpdateAnimation();

	return NodeStatus::Running;
}

NodeStatus Enemy::AttackIDLEAction()
{
	if (GetCurrFrame() >= GetImage()->getMaxFrame() - 1)
	{
		return NodeStatus::Success;
	}
	ObjectRigidBody->Update();
	UpdateAnimation();
	return NodeStatus::Running;
}
