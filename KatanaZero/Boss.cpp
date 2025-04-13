#include "Boss.h"
#include "Image.h"
#include "RenderManager.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

HRESULT Boss::Init()
{
	pos = { 540, 400 };
	wallPos = {190, 200};
	ceilPos = { 540, 100 };
	health = 10;
	isDead = false;
	isAttacked = false;
	state = State::Idle;
	isFlip = false;
	LoopCount = 0;
	framecount = 0;
	timer = 0;

	image = ImageManager::GetInstance()->AddImage("Idle", L"Image/headhunter_idle.bmp",540, 46, 12, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Lazer", L"Image/headhunter_lazer.bmp", 972, 46, 18, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Takerifle", L"Image/headhunter_takeoutrifle.bmp", 405, 45, 9, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Shoot", L"Image/headhunter_shoot.bmp", 385, 55, 7, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Faint", L"Image/headhunter_faint.bmp", 495, 45, 11, 1, true, RGB(255, 0, 255));

	image = ImageManager::GetInstance()->AddImage("Jump", L"Image/headhunter_jump.bmp", 27, 44, 1, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Jumpidle",L"Image/headhunter_jumpidle.bmp", 90, 43, 3, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Jumpto", L"Image/headhunter_jumptoceil.bmp", 35, 46, 1, 1, true, RGB(255, 0, 255));
	return S_OK;
}

void Boss::Release()
{
}

void Boss::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);

	switch (state) {
	case State::Idle:
		image = ImageManager::GetInstance()->FindImage("Idle");
		Idle();
		break;

	case State::TakeOutRifle:
		image = ImageManager::GetInstance()->FindImage("Takerifle");
		TakeOutRifleAttack();
		break;

	case State::Jump:
		Jump();
		break;

	case State::JumpToCeil:
		JumpToCeil();
		break;

	case State::Lazer:
		image = ImageManager::GetInstance()->FindImage("Lazer");
		LazerAttack();
		break;

	case State::Sword:
		SwordAttack(nullptr);
		break;

	case State::Bullet:
		image = ImageManager::GetInstance()->FindImage("Shoot");
		BulletAttack();
		break;

	case State::Faint:
		image = ImageManager::GetInstance()->FindImage("Faint");
		FaintAttack();
		break;

	case State::Teleport:
		break;

	case State::Explode:
		break;

	case State::Die:
		break;

	}

}

void Boss::Render(HDC hdc)
{
	if(image)
	image->FrameRender(hdc, pos.x, pos.y, framecount, 0, isFlip);
}

void Boss::Idle()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();

	if (timer > 0.05f)
	{
		framecount++;
		timer = 0;

		if (framecount >= image->GetMaxFrameX())
		{
			framecount = 0;
			LoopCount++;

			if (LoopCount >= 2)
			{
				LoopCount = 0; 
				state = State::TakeOutRifle;
			}
		}
	}
	//timer += TimerManager::GetInstance()->GetDeltaTime();
	//if (timer > 0.05f)
	//{
	//	framecount++;
	//	timer = 0;

	//	if (framecount >= image->GetMaxFrameX())
	//	{
	//		framecount = 0;
	//		state = State::TakeOutRifle;
	//	}
	//}
}

void Boss::LazerAttack()
{
	// 이미지 재생
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > 0.05f)
	{
		framecount++;
		timer = 0;

	}
	if(framecount >= image->GetMaxFrameX())
	{
		framecount = image->GetMaxFrameX();
		//state = State::바닥으로 슉 이동하는 상태;
	}
	// 레이저 이미지 불러오기? // Image* lazer 별도로 하기
}


void Boss::TakeOutRifleAttack()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > 0.05f)
	{
		framecount++;
		timer = 0;
		if (isAttacked)
		{
			framecount = -1;
			state = State::Faint;
			isAttacked = false;
		}
		if (framecount >= image->GetMaxFrameX())
		{
			framecount = 0;
			state = State::Jump;
		}
	}
}

void Boss::Jump()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();

	if (pos.x > wallPos.x) // 바닥에서 벽으로 곡선 이동
	{	
		image = ImageManager::GetInstance()->FindImage("Jump");
		if (timer > 0.05f)
		{	
			framecount = 0;
			
			float dx = wallPos.x - pos.x;
			float dy = wallPos.y - pos.y;

			float angleRad = atan2(dy, dx);

			pos.x += cos(angleRad) * 30.0f;
			pos.y += sin(angleRad) * 60.0f;

			timer = 0;
		}
	}
		
	if (pos.x <= wallPos.x) // 벽 idle
	{
		pos.x = wallPos.x;
		image = ImageManager::GetInstance()->FindImage("Jumpidle");
		if (timer > 0.1f)
		{
			framecount++;
			timer = 0;
		}
		if (framecount >= image->GetMaxFrameX())
		{
			framecount = 0;
			timer = 0;
			state = State::JumpToCeil;
		}

		
	}
}

void Boss::JumpToCeil()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	image = ImageManager::GetInstance()->FindImage("Jumpto");
	if (timer > 0.05f)
	{
		framecount = 0;

		float dx = ceilPos.x - pos.x;
		float dy = ceilPos.y - pos.y;

		float angleRad = atan2(dy, dx);

		pos.x += cos(angleRad) * 30;
		pos.y += sin(angleRad) * 60;

		timer = 0;
		if (pos.x > ceilPos.x)
		{
			state = State::Lazer;
		}
	}
	
	
}
	


void Boss::SwordAttack(GameObject* player)
{
}

void Boss::BulletAttack()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > 0.1f)
	{
		framecount++;
		timer = 0;

		if (framecount >= image->GetMaxFrameX())
		{
			framecount = -1;
			state = State::Faint;
		}
	}
}

void Boss::FaintAttack()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > 0.1f)
	{
		framecount++;
		pos.x -= 3;
		timer = 0;

		if (framecount >= image->GetMaxFrameX())
		{
			framecount = -1;
			state = State::Lazer;
		}
	}

}

void Boss::Teleport()
{
}

void Boss::Explode()
{
}

void Boss::Die()
{
}
