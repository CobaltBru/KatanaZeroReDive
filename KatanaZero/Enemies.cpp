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
	auto idleaction = bind(&Grunt::IDLEAction, this);
	auto patrolaction = bind(&Grunt::PatrolAction, this);
	auto deadaction = bind(&Grunt::DeadAction, this);
	auto meleeAttackaction = bind(&Grunt::MeleeAttackAction, this);
	auto chaseaction = bind(&Grunt::ChaseAction, this);
	auto findpathaction = bind(&Grunt::FindPathAction, this);
	auto watingaction = bind(&Grunt::WatingAction, this);
	root = new Selector();
	Sequence* Dead = new Sequence();
	Sequence* MeleeAttack = new Sequence();
	Selector* Chase = new Selector();
	Sequence* Waiting = new Sequence();
	Sequence* Patrol = new Sequence();
	Sequence* IDLE = new Sequence();
	root->addChild(Dead);
	root->addChild(MeleeAttack);
	root->addChild(Chase);
	root->addChild(Patrol);
	root->addChild(IDLE);

	ConditionNode* isDead = new ConditionNode([this]() {
		return this->bDead;
		});
	ActionNode* changeDeadAnim = new ActionNode("changeDead", [this]() {
		this->ChangeAnimation(EImageType::Dead);
		return NodeStatus::Success; });
	ActionNode* DeadAction = new ActionNode("Dead", deadaction);
	Dead->addChild(isDead);
	Dead->addChild(changeDeadAnim);
	Dead->addChild(DeadAction);

	ConditionNode* IsInAttackRange = new ConditionNode([this]() {
		return this->IsInAttackRange();
		});
	ConditionNode* CanAttack = new ConditionNode([this]() {
		return this->attackTimer == 0.f;
		});
	ActionNode* changeAttackAnim = new ActionNode("changeAttack", [this]() {
		this->ChangeAnimation(EImageType::Attack);
		return NodeStatus::Success; });
	ActionNode* MeleeAttackAction = new ActionNode("Attack", meleeAttackaction);
	MeleeAttack->addChild(IsInAttackRange);
	MeleeAttack->addChild(CanAttack);
	MeleeAttack->addChild(changeAttackAnim);
	MeleeAttack->addChild(MeleeAttackAction);

	Sequence* DirectChase = new Sequence();
	ConditionNode* IsInSameFloor = new ConditionNode([this]() { return false; });
	ActionNode* changeChaseAnim = new ActionNode("changeChase", [this]() {
		this->ChangeAnimation(EImageType::Run);
		return NodeStatus::Success; });
	ActionNode* ChaseAction = new ActionNode("Chase", chaseaction);
	DirectChase->addChild(IsInSameFloor);
	DirectChase->addChild(changeChaseAnim);
	DirectChase->addChild(ChaseAction);
	Sequence* FindPath = new Sequence();
	ConditionNode* CanFindPath = new ConditionNode([this]() { return false; });
	ActionNode* MoveToPath = new ActionNode("FindPath", findpathaction);
	FindPath->addChild(CanFindPath);
	FindPath->addChild(changeChaseAnim);
	FindPath->addChild(MoveToPath);

	Chase->addChild(DirectChase);
	Chase->addChild(FindPath);


	ActionNode* changeWaitingAnim = new ActionNode("changeWait", [this]() {
		this->ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success; });
	ActionNode* WaitingAction = new ActionNode("Waiting", watingaction);
	Waiting->addChild(changeWaitingAnim);
	Waiting->addChild(WaitingAction);
	Chase->addChild(Waiting);

	ConditionNode* CanPatrol = new ConditionNode([this]() { return this->canPatrol; });
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
	attackRange = 50.f;
	attackDuration = 0.5f;
	eType = EType::Grunt;
	AttackCollider = new Collider(this, EColliderType::Rect, {}, attackRange, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	AttackCollider->SetPivot({ attackRange / 2.f, 0.f });
	AttackCollider->SetPos({ Pos.x, Pos.y });
	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
	ObjectCollider->SetPos(Pos);

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

	ObjectRigidBody = new RigidBody(this);
	
	InitRigidBodySetting();

	// BT 세팅
	auto idleaction = bind(&Grunt::IDLEAction, this);
	auto patrolaction = bind(&Grunt::PatrolAction, this);
	auto deadaction = bind(&Grunt::DeadAction, this);
	auto meleeAttackaction = bind(&Grunt::MeleeAttackAction, this);
	auto chaseaction = bind(&Grunt::ChaseAction, this);
	auto findpathaction = bind(&Grunt::FindPathAction, this);
	auto watingaction = bind(&Grunt::WatingAction, this);
	root = new Selector();
	Sequence* Dead = new Sequence();
	Sequence* MeleeAttack = new Sequence();
	Selector* Chase = new Selector();
	Sequence* Waiting = new Sequence();
	Sequence* Patrol = new Sequence();
	Sequence* IDLE = new Sequence();
	root->addChild(Dead);
	root->addChild(MeleeAttack);
	root->addChild(Chase);
	root->addChild(Patrol);
	root->addChild(IDLE);

	ConditionNode* isDead = new ConditionNode([this]() {
		return this->bDead;
		});
	ActionNode* changeDeadAnim = new ActionNode("changeDead", [this]() {
		this->ChangeAnimation(EImageType::Dead);
		return NodeStatus::Success; });
	ActionNode* DeadAction = new ActionNode("Dead", deadaction);
	Dead->addChild(isDead);
	Dead->addChild(changeDeadAnim);
	Dead->addChild(DeadAction);

	ConditionNode* IsInAttackRange = new ConditionNode([this]() {
		return this->IsInAttackRange();
		});
	ConditionNode* CanAttack = new ConditionNode([this]() {
		return this->attackTimer == 0.f;
		});
	ActionNode* changeAttackAnim = new ActionNode("changeAttack", [this]() {
		this->ChangeAnimation(EImageType::Attack);
		return NodeStatus::Success; });
	ActionNode* MeleeAttackAction = new ActionNode("Attack", meleeAttackaction);
	MeleeAttack->addChild(IsInAttackRange);
	MeleeAttack->addChild(CanAttack);
	MeleeAttack->addChild(changeAttackAnim);
	MeleeAttack->addChild(MeleeAttackAction);

	Sequence* DirectChase = new Sequence();
	ConditionNode* IsInSameFloor = new ConditionNode([this]() { return false; });
	ActionNode* changeChaseAnim = new ActionNode("changeChase", [this]() {
		this->ChangeAnimation(EImageType::Run);
		return NodeStatus::Success; });
	ActionNode* ChaseAction = new ActionNode("Chase", chaseaction);
	DirectChase->addChild(IsInSameFloor);
	DirectChase->addChild(changeChaseAnim);
	DirectChase->addChild(ChaseAction);
	Sequence* FindPath = new Sequence();
	ConditionNode* CanFindPath = new ConditionNode([this]() { return false; });
	ActionNode* MoveToPath = new ActionNode("FindPath", findpathaction);
	FindPath->addChild(CanFindPath);
	FindPath->addChild(changeChaseAnim);
	FindPath->addChild(MoveToPath);

	Chase->addChild(DirectChase);
	Chase->addChild(FindPath);

	ConditionNode* IsChasing = new ConditionNode([this]() { return bChasing; });
	ActionNode* changeWaitingAnim = new ActionNode("changeWait", [this]() {
		this->ChangeAnimation(EImageType::IDLE);
		return NodeStatus::Success; });
	ActionNode* WaitingAction = new ActionNode("Waiting", watingaction);
	Waiting->addChild(IsChasing);
	Waiting->addChild(changeWaitingAnim);
	Waiting->addChild(WaitingAction);
	Chase->addChild(Waiting);

	ConditionNode* CanPatrol = new ConditionNode([this]() { return this->canPatrol; });
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

NodeStatus Grunt::IDLEAction()
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

NodeStatus Grunt::PatrolAction()
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

NodeStatus Grunt::DeadAction()
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

NodeStatus Grunt::MeleeAttackAction()
{
	if (GetCurrFrame() >= GetImage()->getMaxFrame() - 1)
	{
		return NodeStatus::Success;
	}
	Collision();
	ObjectRigidBody->Update();
	UpdateAnimation();
	return NodeStatus::Running;
}

NodeStatus Grunt::ChaseAction()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return NodeStatus::Failure;
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
	
	return NodeStatus::Running;
}

NodeStatus Grunt::FindPathAction()
{
	// 이거 로직 바꿔야되는데...
	// 추적 자체는 레이로 하는데
	// 경로 탐색 로직 자체를 다 뜯어야된다

	return NodeStatus::Success;
}

NodeStatus Grunt::WatingAction()
{
	if (GetCurrFrame() >= GetImage()->getMaxFrame() - 1)
	{
		return NodeStatus::Success;
	}
	ObjectRigidBody->Update();
	UpdateAnimation();
	
	return NodeStatus::Running;
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
	AttackCollider = new Collider(this, EColliderType::Rect, {}, attackRange, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(AttackCollider, ECollisionGroup::Enemy);
	AttackCollider->SetPivot({ attackRange / 2.f, 0.f });
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


