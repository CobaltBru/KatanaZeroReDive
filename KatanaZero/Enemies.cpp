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
#include "LineManager.h"
#include "PathFinder.h"

HRESULT Grunt::Init(FPOINT InPos)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	ObjectCollider = new Collider(this, EColliderType::Rect, {}, 30.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	
	ObjectCollider->SetPos(Pos);
	ObjectRigidBody = new RigidBody(this);
	Speed = 200.f;
	detectRange = 400.f;
	attackRange = 50.f;
	attackDuration = 0.5f;
	eType = EType::Grunt;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, attackRange, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	AttackCollider->SetPivot({ attackRange / 2.f, 0.f });
	AttackCollider->SetPos({ Pos.x, Pos.y });
	InitRigidBodySetting();

	// BT 세팅
	auto idleaction = bind(&Enemy::IDLEAction, this);
	auto patrolaction = bind(&Enemy::PatrolAction, this);
	auto deadaction = bind(&Enemy::DeadAction, this);
	auto meleeAttackaction = bind(&Enemy::MeleeAttackAction, this);
	auto attackIDLEaction = bind(&Enemy::AttackIDLEAction, this);
	auto chaseaction = bind(&Enemy::ChaseAction, this);
	auto calcpathaction = bind(&Enemy::CalcPathAction, this);
	auto findpathaction = bind(&Enemy::FindPathAction, this);
	auto watingaction = bind(&Enemy::WatingAction, this);
	root = new Selector();
	Sequence* Dead = new Sequence();
	Sequence* MeleeAttack = new Sequence();
	Selector* Chase = new Selector();

	Sequence* Patrol = new Sequence();
	Sequence* IDLE = new Sequence();
	root->addChild(Dead);
	root->addChild(MeleeAttack);
	root->addChild(Chase);
	root->addChild(Patrol);
	root->addChild(IDLE);

	ConditionNode* isDead = new ConditionNode([this]() {
		return this->bHitted;
		});
	ActionNode* changeDeadAnim = new ActionNode("changeDead", [this]() {
		this->ChangeAnimation(EImageType::Dead);
		return NodeStatus::Success; });
	ActionNode* DeadAction = new ActionNode("Dead", deadaction);
	Dead->addChild(isDead);
	Dead->addChild(changeDeadAnim);
	Dead->addChild(DeadAction);


	ConditionNode* IsInAttackRange = new ConditionNode([this]() {
		return (this->IsInAttackRange() || this->bAttacking) && !bHitted;
		});
	MeleeAttack->addChild(IsInAttackRange);
	ConditionNode* CanAttack = new ConditionNode([this]() {
		return this->attackTimer == 0.f;
		});
	ActionNode* changeAttackAnim = new ActionNode("changeAttack", [this]() {
		this->ChangeAnimation(EImageType::Attack);

		bAttacking = true;
		return NodeStatus::Success; });
	ActionNode* MeleeAttackAction = new ActionNode("Attack", meleeAttackaction);
	ActionNode* changeAttackIDLEAnim = new ActionNode("changeAttackIDLE", [this]() { this->ChangeAnimation(EImageType::IDLE);
	return NodeStatus::Success; });
	ActionNode* AttackIDLEAction = new ActionNode("AttackIDLE", attackIDLEaction);

	Selector* AttackBehavior = new Selector();
	Sequence* DoAttack = new Sequence();
	DoAttack->addChild(CanAttack);
	DoAttack->addChild(changeAttackAnim);
	DoAttack->addChild(MeleeAttackAction);

	Sequence* DoAttackIDLE = new Sequence();
	DoAttackIDLE->addChild(changeAttackIDLEAnim);
	DoAttackIDLE->addChild(AttackIDLEAction);

	AttackBehavior->addChild(DoAttack);
	AttackBehavior->addChild(DoAttackIDLE);

	MeleeAttack->addChild(AttackBehavior);

	Sequence* DirectChase = new Sequence();
	ConditionNode* IsInSameFloor = new ConditionNode([this]() { return Detecting(); });
	ActionNode* changeChaseAnim = new ActionNode("changeChase", [this]() {
		this->ChangeAnimation(EImageType::Run);
		return NodeStatus::Success; });
	ActionNode* ChaseAction = new ActionNode("Chase", chaseaction);
	DirectChase->addChild(IsInSameFloor);
	DirectChase->addChild(changeChaseAnim);
	DirectChase->addChild(ChaseAction);

	Sequence* FindPath = new Sequence();
	ConditionNode* CanFindPath = new ConditionNode([this]() { return !Detecting() && bChasing; });
	Node* CalcPath = new CooldownNode(new ActionNode("CalcPath", calcpathaction), recalcDuration);
	ConditionNode* HasPath = new ConditionNode([this]() { return !navPath.isEmpty(); });
	ActionNode* MoveToPath = new ActionNode("FindPath", findpathaction);
	FindPath->addChild(CanFindPath);
	FindPath->addChild(CalcPath);
	FindPath->addChild(HasPath);
	FindPath->addChild(MoveToPath);

	Chase->addChild(DirectChase);
	Chase->addChild(FindPath);

	Sequence* Waiting = new Sequence();
	ConditionNode* IsChasing = new ConditionNode([this]() { return bChasing && !Detecting() && navPath.isEmpty(); });
	ActionNode* changeWaitingAnim = new ActionNode("changeWait", [this]() {
		this->ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success; });
	ActionNode* WaitingAction = new ActionNode("Waiting", watingaction);
	Waiting->addChild(IsChasing);
	Waiting->addChild(changeWaitingAnim);
	Waiting->addChild(WaitingAction);
	Chase->addChild(Waiting);

	ConditionNode* CanPatrol = new ConditionNode([this]() { return this->canPatrol && !bHitted; });
	ActionNode* changePatrolAnim = new ActionNode("changePatrol", [this]() {
		this->ChangeAnimation(EImageType::Walk);
		return NodeStatus::Success; });
	ActionNode* PatrolAction = new ActionNode("Patrol", patrolaction);
	Patrol->addChild(CanPatrol);
	Patrol->addChild(changePatrolAnim);
	Patrol->addChild(PatrolAction);

	ActionNode* changeIDLEAnim = new ActionNode("changeIDLE", [this]() {
		this->ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success;
		});
	ActionNode* IDLEaction = new ActionNode("IDLE", idleaction);
	IDLE->addChild(changeIDLEAnim);
	IDLE->addChild(IDLEaction);
	// BT 세팅 끝
	return S_OK;
}

HRESULT Grunt::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImages();
	Pos = InPos;
	Speed = 200.f;
	detectRange = 400.f;
	attackRange = 60.f;
	attackDuration = 0.5f;
	eType = EType::Grunt;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, attackRange * 2.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	baseAttackPivotX = attackRange / 2.f * dir;
	baseAttackPivotY = 0.f;
	AttackCollider->SetPivot({ baseAttackPivotX, baseAttackPivotY });
	AttackCollider->SetPos({ Pos.x, Pos.y });
	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);
	
	InitRigidBodySetting();

	pathFinder = new PathFinder(LineManager::GetInstance()->GetNodes(), LineManager::GetInstance()->GetGraph());

	// BT 세팅
	auto idleaction = bind(&Enemy::IDLEAction, this);
	auto patrolaction = bind(&Enemy::PatrolAction, this);
	auto deadaction = bind(&Enemy::DeadAction, this);
	auto meleeAttackaction = bind(&Enemy::MeleeAttackAction, this);
	auto attackIDLEaction = bind(&Enemy::AttackIDLEAction, this);
	auto chaseaction = bind(&Enemy::ChaseAction, this);
	auto calcpathaction = bind(&Enemy::CalcPathAction, this);
	auto findpathaction = bind(&Enemy::FindPathAction, this);
	auto watingaction = bind(&Enemy::WatingAction, this);
	root = new Selector();
	Sequence* Dead = new Sequence();
	Sequence* MeleeAttack = new Sequence();
	Selector* Chase = new Selector();
	
	Sequence* Patrol = new Sequence();
	Sequence* IDLE = new Sequence();
	root->addChild(Dead);
	root->addChild(MeleeAttack);
	root->addChild(Chase);
	root->addChild(Patrol);
	root->addChild(IDLE);

	ConditionNode* isDead = new ConditionNode([this]() {
		return this->bHitted;
		});
	ActionNode* changeDeadAnim = new ActionNode("changeDead", [this]() {
		this->ChangeAnimation(EImageType::Dead);
		return NodeStatus::Success; });
	ActionNode* DeadAction = new ActionNode("Dead", deadaction);
	Dead->addChild(isDead);
	Dead->addChild(changeDeadAnim);
	Dead->addChild(DeadAction);


	ConditionNode* IsInAttackRange = new ConditionNode([this]() {
		return (this->IsInAttackRange() || this->bAttacking) && !bHitted;
		});
	MeleeAttack->addChild(IsInAttackRange);
	ConditionNode* CanAttack = new ConditionNode([this]() {
		return this->attackTimer == 0.f;
		});
	ActionNode* changeAttackAnim = new ActionNode("changeAttack", [this]() {
		this->ChangeAnimation(EImageType::Attack);
		
		bAttacking = true;
		return NodeStatus::Success; });
	ActionNode* MeleeAttackAction = new ActionNode("Attack", meleeAttackaction);
	ActionNode* changeAttackIDLEAnim = new ActionNode("changeAttackIDLE", [this]() { this->ChangeAnimation(EImageType::IDLE);
	return NodeStatus::Success; });
	ActionNode* AttackIDLEAction = new ActionNode("AttackIDLE", attackIDLEaction);

	Selector* AttackBehavior = new Selector();
	Sequence* DoAttack = new Sequence();
	DoAttack->addChild(CanAttack);
	DoAttack->addChild(changeAttackAnim);
	DoAttack->addChild(MeleeAttackAction);

	Sequence* DoAttackIDLE = new Sequence();
	DoAttackIDLE->addChild(changeAttackIDLEAnim);
	DoAttackIDLE->addChild(AttackIDLEAction);

	AttackBehavior->addChild(DoAttack);
	AttackBehavior->addChild(DoAttackIDLE);

	MeleeAttack->addChild(AttackBehavior);

	Sequence* DirectChase = new Sequence();
	ConditionNode* IsInSameFloor = new ConditionNode([this]() { return Detecting(); });
	ActionNode* changeChaseAnim = new ActionNode("changeChase", [this]() {
		this->ChangeAnimation(EImageType::Run);
		return NodeStatus::Success; });
	ActionNode* ChaseAction = new ActionNode("Chase", chaseaction);
	DirectChase->addChild(IsInSameFloor);
	DirectChase->addChild(changeChaseAnim);
	DirectChase->addChild(ChaseAction);

	Sequence* FindPath = new Sequence();
	ConditionNode* CanFindPath = new ConditionNode([this]() { return !Detecting() && bChasing; });
	Node* CalcPath = new CooldownNode(new ActionNode("CalcPath", calcpathaction), recalcDuration);
	ConditionNode* HasPath = new ConditionNode([this]() { return !navPath.isEmpty(); });
	ActionNode* MoveToPath = new ActionNode("FindPath", findpathaction);
	FindPath->addChild(CanFindPath);
	FindPath->addChild(CalcPath);
	FindPath->addChild(HasPath);
	FindPath->addChild(MoveToPath);

	Chase->addChild(DirectChase);
	Chase->addChild(FindPath);

	Sequence* Waiting = new Sequence();
	ConditionNode* IsChasing = new ConditionNode([this]() { return bChasing && !Detecting() && navPath.isEmpty(); });
	ActionNode* changeWaitingAnim = new ActionNode("changeWait", [this]() {
		this->ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success; });
	ActionNode* WaitingAction = new ActionNode("Waiting", watingaction);
	Waiting->addChild(IsChasing);
	Waiting->addChild(changeWaitingAnim);
	Waiting->addChild(WaitingAction);
	Chase->addChild(Waiting);

	ConditionNode* CanPatrol = new ConditionNode([this]() { return this->canPatrol && !bHitted; });
	ActionNode* changePatrolAnim = new ActionNode("changePatrol", [this]() {
		this->ChangeAnimation(EImageType::Walk);
		return NodeStatus::Success; });
	ActionNode* PatrolAction = new ActionNode("Patrol", patrolaction);
	Patrol->addChild(CanPatrol);
	Patrol->addChild(changePatrolAnim);
	Patrol->addChild(PatrolAction);

	ActionNode* changeIDLEAnim = new ActionNode("changeIDLE", [this]() {
		this->ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success;
		});
	ActionNode* IDLEaction = new ActionNode("IDLE", idleaction);
	IDLE->addChild(changeIDLEAnim);
	IDLE->addChild(IDLEaction);
	// BT 세팅 끝

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
	Speed = 250.f;
	detectRange = 600.f;
	attackRange = 60.f;
	attackDuration = 0.3f;
	eType = EType::Pomp;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, attackRange, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	AttackCollider->SetPivot({ attackRange / 2.f, 0.f });
	AttackCollider->SetPos({ Pos.x, Pos.y });
	InitRigidBodySetting();
	return S_OK;
}

HRESULT Pomp::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	Speed = 250.f;
	detectRange = 600.f;
	attackRange = 60.f;
	attackDuration = 0.3f;
	eType = EType::Pomp;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, attackRange * 2.f, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	baseAttackPivotX = attackRange / 2.f * dir;
	baseAttackPivotY = 0.f;
	AttackCollider->SetPivot({ baseAttackPivotX, baseAttackPivotY });
	AttackCollider->SetPos({ Pos.x, Pos.y });

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);

	InitRigidBodySetting();
	pathFinder = new PathFinder(LineManager::GetInstance()->GetNodes(), LineManager::GetInstance()->GetGraph());
	auto idleaction = bind(&Enemy::IDLEAction, this);
	auto patrolaction = bind(&Enemy::PatrolAction, this);
	auto deadaction = bind(&Enemy::DeadAction, this);
	auto meleeAttackaction = bind(&Enemy::MeleeAttackAction, this);
	auto attackIDLEaction = bind(&Enemy::AttackIDLEAction, this);
	auto chaseaction = bind(&Enemy::ChaseAction, this);
	auto calcpathaction = bind(&Enemy::CalcPathAction, this);
	auto findpathaction = bind(&Enemy::FindPathAction, this);
	auto watingaction = bind(&Enemy::WatingAction, this);
	auto parryingaction = bind(&Pomp::ParryingAction, this);

	root = new Selector();

	Sequence* Parrying = new Sequence();
	Sequence* Dead = new Sequence();
	Sequence* MeleeAttack = new Sequence();
	Selector* Chase = new Selector();
	Sequence* Patrol = new Sequence();
	Sequence* IDLE = new Sequence();

	// 트리 구조 연결
	root->addChild(Parrying);
	root->addChild(Dead);
	root->addChild(MeleeAttack);
	root->addChild(Chase);
	root->addChild(Patrol);
	root->addChild(IDLE);

	// --- Parrying 시퀀스 ---
	ConditionNode* CanParrying = new ConditionNode([this]() {
		return bAttacking && ObjectCollider->IsHitted() || bParrying;
		});
	ActionNode* changeParryingAnim = new ActionNode("changeParrying", [this]() {
		bParrying = true;
		this->ChangeAnimation(EImageType::Dead);  // 피격 무효시 튕겨나갈 때 Dead 애니 사용
		ObjectCollider->SetHit(false);
		AttackCollider->SetHit(false);
		bHitted = false;
		bAttacking = false;
		return NodeStatus::Success;
		});
	ActionNode* doParrying = new ActionNode("Parrying", parryingaction);

	Parrying->addChild(CanParrying);
	Parrying->addChild(changeParryingAnim);
	Parrying->addChild(doParrying);

	// --- Dead 시퀀스 ---
	ConditionNode* isDead = new ConditionNode([this]() {
		return bHitted && !bParrying;
		});
	ActionNode* changeDeadAnim = new ActionNode("changeDead", [this]() {
		this->ChangeAnimation(EImageType::Dead);
		return NodeStatus::Success;
		});
	ActionNode* DeadAction = new ActionNode("Dead", deadaction);

	Dead->addChild(isDead);
	Dead->addChild(changeDeadAnim);
	Dead->addChild(DeadAction);

	// --- MeleeAttack 시퀀스 ---
	ConditionNode* IsInAttackRange = new ConditionNode([this]() {
		return (this->IsInAttackRange() || bAttacking) && !bHitted && !bParrying;
		});
	ConditionNode* CanAttack = new ConditionNode([this]() {
		return attackTimer == 0.f;
		});
	ActionNode* changeAttackAnim = new ActionNode("changeAttack", [this]() {
		ChangeAnimation(EImageType::Attack);
		bAttacking = true;
		return NodeStatus::Success;
		});
	ActionNode* MeleeAttackAction = new ActionNode("Attack", meleeAttackaction);
	ActionNode* changeAttackIDLEAnim = new ActionNode("changeAttackIDLE", [this]() {
		ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success;
		});
	ActionNode* AttackIDLEAction = new ActionNode("AttackIDLE", attackIDLEaction);

	Selector* AttackBehavior = new Selector();
	Sequence* DoAttack = new Sequence();
	Sequence* DoAttackIDLE = new Sequence();

	DoAttack->addChild(CanAttack);
	DoAttack->addChild(changeAttackAnim);
	DoAttack->addChild(MeleeAttackAction);

	DoAttackIDLE->addChild(changeAttackIDLEAnim);
	DoAttackIDLE->addChild(AttackIDLEAction);

	AttackBehavior->addChild(DoAttack);
	AttackBehavior->addChild(DoAttackIDLE);

	MeleeAttack->addChild(IsInAttackRange);
	MeleeAttack->addChild(AttackBehavior);

	// --- Chase 시퀀스 ---
	Sequence* DirectChase = new Sequence();
	ConditionNode* IsInSameFloor = new ConditionNode([this]() {
		return Detecting() && !bParrying;
		});
	ActionNode* changeChaseAnim = new ActionNode("changeChase", [this]() {
		ChangeAnimation(EImageType::Run);
		return NodeStatus::Success;
		});
	ActionNode* ChaseAction = new ActionNode("Chase", chaseaction);

	DirectChase->addChild(IsInSameFloor);
	DirectChase->addChild(changeChaseAnim);
	DirectChase->addChild(ChaseAction);

	// 분할 추적 (optional CooldownNode 적용 전)
	Sequence* FindPathSeq = new Sequence();
	ConditionNode* CanFindPath = new ConditionNode([this]() {
		return !Detecting() && bChasing && !bParrying;
		});
	ActionNode* CalcPath = new ActionNode("CalcPath", calcpathaction);
	ConditionNode* HasPath = new ConditionNode([this]() {
		return !navPath.isEmpty();
		});
	ActionNode* MoveToPath = new ActionNode("FindPath", findpathaction);

	FindPathSeq->addChild(CanFindPath);
	FindPathSeq->addChild(CalcPath);
	FindPathSeq->addChild(HasPath);
	FindPathSeq->addChild(MoveToPath);

	Chase->addChild(DirectChase);
	Chase->addChild(FindPathSeq);

	// Waiting 상태: 추적 중 경로 없을 때
	Sequence* Waiting = new Sequence();
	ConditionNode* IsChasing = new ConditionNode([this]() {
		return bChasing && !Detecting() && navPath.isEmpty() && !bParrying;
		});
	ActionNode* changeWaitingAnim = new ActionNode("changeWait", [this]() {
		ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success;
		});
	ActionNode* WaitingAction = new ActionNode("Waiting", watingaction);

	Waiting->addChild(IsChasing);
	Waiting->addChild(changeWaitingAnim);
	Waiting->addChild(WaitingAction);
	Chase->addChild(Waiting);

	// --- Patrol & Idle ---
	ConditionNode* CanPatrol = new ConditionNode([this]() {
		return canPatrol && !bHitted;
		});
	ActionNode* changePatrolAnim = new ActionNode("changePatrol", [this]() {
		ChangeAnimation(EImageType::Walk);
		return NodeStatus::Success;
		});
	ActionNode* PatrolAction = new ActionNode("Patrol", patrolaction);
	Patrol->addChild(CanPatrol);
	Patrol->addChild(changePatrolAnim);
	Patrol->addChild(PatrolAction);

	ActionNode* changeIDLEAnim = new ActionNode("changeIDLE", [this]() {
		ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success;
		});
	ActionNode* IDLEaction = new ActionNode("IDLE", idleaction);
	IDLE->addChild(changeIDLEAnim);
	IDLE->addChild(IDLEaction);
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

NodeStatus Pomp::ParryingAction()
{
	if (currFrame >= GetImage()->getMaxFrame() - 1)
	{
		bParrying = false;
		return NodeStatus::Success;
	}

	// 뒤로 튕겨나가기 + 공중 모션
	ObjectRigidBody->AddVelocity({ -dir * 100.f, -100.f });
	ObjectRigidBody->Update();
	UpdateAnimation();
	return NodeStatus::Running;
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
	Speed = 150.f;
	detectRange = 500.f;
	attackRange = 400.f;
	meleeAttackRange = 50.f;
	attackDuration = 1.2f;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, meleeAttackRange, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	AttackCollider->SetPivot({ meleeAttackRange / 2.f, 0.f });
	AttackCollider->SetPos({ Pos.x, Pos.y });
	eType = EType::Gangster;
	InitRigidBodySetting();
	return S_OK;
}

HRESULT Gangster::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	InitImages();
	eState = new EIDLE();
	Pos = InPos;
	Speed = 150.f;
	detectRange = 500.f;
	attackRange = 400.f;
	meleeAttackRange = 50.f;
	attackDuration = 1.2f;
	eType = EType::Gangster;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, meleeAttackRange, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	AttackCollider->SetPivot({ meleeAttackRange / 2.f, 0.f });
	AttackCollider->SetPos({ Pos.x, Pos.y });
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


