#include "Boss.h"
#include "Image.h"
#include "RenderManager.h"
#include "CommonFunction.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

HRESULT Boss::Init()
{
	// 맵 가로 240~840, 세로 140~360  
	pos = { 800, 360 };
	ceilPos = {};
	wallPos = {};
	landPos = {};
	playerPos = {700 ,360};
	isRetreating = false;
	health = 4;
	isDead = false;
	isAttacked = false;
	angle = 225;
	//movingDistance = 0.0f;

	state = State::Idle;
	isFlip = false;
	isLeft = false;
	LoopCount = 0;
	frameIndex = 0;
	timer = 0;

	image = ImageManager::GetInstance()->AddImage("Idle", L"Image/headhunter_idle.bmp",540, 46, 12, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Lazer", L"Image/headhunter_lazer.bmp", 972, 46, 18, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Takerifle", L"Image/headhunter_takeoutrifle.bmp", 405, 45, 9, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Faint", L"Image/headhunter_faint.bmp", 495, 45, 11, 1, true, RGB(255, 0, 255));

	//BULLET
	image = ImageManager::GetInstance()->AddImage("Jump", L"Image/headhunter_jump.bmp", 27, 44, 1, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Jumpidle",L"Image/headhunter_jumpidle.bmp", 90, 43, 3, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Jumpto", L"Image/headhunter_jumptoceil.bmp", 35, 46, 1, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Shoot", L"Image/headhunter_shoot.bmp", 385, 55, 7, 1, true, RGB(255, 0, 255));

	//DASH
	image = ImageManager::GetInstance()->AddImage("Readytodash", L"Image/readytodash.bmp", 432, 45, 9, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Dash", L"Image/dash.bmp", 51, 25, 1, 1, true, RGB(255, 0, 255));
	image = ImageManager::GetInstance()->AddImage("Dashend", L"Image/dashend.bmp", 500, 50, 10, 1, true, RGB(255, 0, 255));


	return S_OK;
}

void Boss::Release()
{
}

void Boss::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
	CheckPlayerPos();
	CheckLeft();
	switch (state) {
	case State::Idle:
		Idle();
		break;

	case State::TakeOutRifle:
		TakeOutRifleAttack();
		break;

	case State::Jump:
		Jump();
		break;

	case State::JumpIdle:
		image = ImageManager::GetInstance()->FindImage("Jumpidle");
		timer += TimerManager::GetInstance()->GetDeltaTime();
		//if (isLeft)
		//{
		//	isFlip = false;
		//}
		//else
		//{
		//	isFlip = true;
		//}
		if (timer > 0.1f)
		{
			frameIndex++;
			timer = 0;
			if (frameIndex >= image->GetMaxFrameX())
			{
				frameIndex = 0;
				state = State::JumpToCeil;
			}
		}
		break;

	case State::JumpToCeil:
		JumpToCeil();
		break;

	case State::Lazer:
		image = ImageManager::GetInstance()->FindImage("Lazer");
		LazerAttack();
		break;

	case State::ReadyToDash:
		ReadyToDash();
		break;

	case State::Dash:
		Dash();
		break;

	case State::DashEnd:
		DashEnd();
		break;

	case State::Bullet:
		image = ImageManager::GetInstance()->FindImage("Shoot");
		BulletAttack();
		break;

	case State::Faint:
		image = ImageManager::GetInstance()->FindImage("Faint");
		Faint();
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
	//RenderRectAtCenter(hdc, WINSIZE_X/2,WINSIZE_Y/2, 640, 260);
	RenderRectAtCenter(hdc, playerPos.x, playerPos.y, 20, 40);
	image->FrameRender(hdc, pos.x, pos.y, frameIndex, 0, isFlip);

}

void Boss::Idle()
{
	image = ImageManager::GetInstance()->FindImage("Idle");
	timer += TimerManager::GetInstance()->GetDeltaTime();
	CheckPlayerPos();
	if (timer > 0.05f)
	{
		frameIndex++;
		timer = 0;

		if (frameIndex >= image->GetMaxFrameX())
		{
			frameIndex = 0;
			LoopCount++;

			if (LoopCount >= 2)
			{
				LoopCount = 0; 
				state = State::TakeOutRifle;
			}
		}
	}
}

void Boss::LazerAttack()
{
	image = ImageManager::GetInstance()->FindImage("Lazer");
	// 이미지 재생
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > 0.05f)
	{
		frameIndex++;
		timer = 0;

	}
	if(frameIndex >= image->GetMaxFrameX())
	{
		frameIndex = image->GetMaxFrameX();
		//state = State::바닥으로 슉 이동하는 상태;
	}
	// 레이저 이미지 불러오기? // Image* lazer 별도로 하기
}


void Boss::TakeOutRifleAttack()
{
	image = ImageManager::GetInstance()->FindImage("Takerifle");
	timer += TimerManager::GetInstance()->GetDeltaTime();

	if (isAttacked)
	{
		frameIndex = -1;
		state = State::Faint;
		isAttacked = false;
	}

	if (frameIndex < image->GetMaxFrameX())
	{
		if (timer > 0.1f)
		{
			frameIndex++;
			timer = 0;
		}
	}

	if (frameIndex >= image->GetMaxFrameX())
	{
		frameIndex = image->GetMaxFrameX();
		if (timer > 0.5f) // 0.5f 값부분 조정해서 총을 쏘는 시간 조절
		{
			frameIndex = 0;
			//isFlip = false;
			state = State::Jump;

		}
	}
}

void Boss::TakeOutWeapon(string key)
{
	image = ImageManager::GetInstance()->FindImage(key);

	//if (isLeft)
	//{
	//	isFlip = false;
	//}
	//else
	//{
	//	isFlip = true;
	//}

	if (isAttacked)
	{
		frameIndex = -1;
		state = State::Faint;
		isAttacked = false;
	}

	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (frameIndex < image->GetMaxFrameX())
	{
		if (timer > 0.1f)
		{
			frameIndex++;
			timer = 0;
		}
	}

	if (frameIndex >= image->GetMaxFrameX())
	{
		frameIndex = image->GetMaxFrameX();
		if (timer > 0.5f) // 0.5f 값부분 조정해서 총을 쏘는 시간 조절
		{
			//isFlip = false;
			state = State::Jump;
		}
	}
}

void Boss::Jump()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	image = ImageManager::GetInstance()->FindImage("Jump");
	if (isLeft)// 보스가 플레이어 기준으로 왼쪽에 있을 경우 왼쪽으로 도약
	{
		if (timer > 0.1f)
		{
			frameIndex = 0;

			pos.x += cos(DEG_TO_RAD(angle)) * 30.0f;
			pos.y += sin(DEG_TO_RAD(angle)) * 30.0f;
			angle -= 2;

			timer = 0;
			if (pos.x <= wallPos.x)
			{
				pos.x = wallPos.x;
				angle = 225;
				frameIndex = 0;
				state = State::JumpIdle;
			}
		}
	}

	if (!isLeft)// 보스 오른편일 경우
	{
		if (timer > 0.1f)
		{

			frameIndex = 0;

			pos.x += cos(DEG_TO_RAD(angle+90)) * 30.0f;
			pos.y += sin(DEG_TO_RAD(angle+90)) * 30.0f;
			angle -= 2;

			timer = 0;

			if (pos.x > wallPos.x +5)
			{
				pos.x = wallPos.x +5;
				//isFlip = false;
				angle = 225;
				frameIndex = 0;
				state = State::JumpIdle;
				}
			}

		}
}

void Boss::JumpToCeil()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	image = ImageManager::GetInstance()->FindImage("Jumpto");
	if (isLeft) 
	{
		if (timer > 0.1f)
			{
				frameIndex = 0;

				pos.x += cos(DEG_TO_RAD(angle+90)) * 30;
				pos.y += sin(DEG_TO_RAD(angle+90)) * 30;
				angle += 2;

				timer = 0;

				if (pos.y <= 250)
				{
					angle = 225;
					state = State::Bullet;
				}
			}
	}
	
	else
	{
		//isFlip = true;
		if (timer > 0.1f)
		{
			frameIndex = 0;

			pos.x += cos(DEG_TO_RAD(angle-20)) * 30;
			pos.y += sin(DEG_TO_RAD(angle-20)) * 30;
			angle += 2;

			timer = 0;

			if (pos.y <= 250 )
			{
				angle = 225;
				state = State::Bullet;
			}
		}
	}
	
	
}
	

void Boss::ReadyToDash()
{
	// 대쉬 이미지 구현
	timer += TimerManager::GetInstance()->GetDeltaTime();
	image = ImageManager::GetInstance()->FindImage("Readytodash");
	if (timer > 0.1f)
	{
		frameIndex++;
		timer = 0;
		if (frameIndex >= image->GetMaxFrameX())
		{
			frameIndex = 0;
			state = State::Dash;
		}
	}


}

void Boss::Dash()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	image = ImageManager::GetInstance()->FindImage("Dash");
	
	if (timer > 0.005f)
	{
		pos.x += 5;
		timer = 0;

		if (pos.x > 820)
		{
			pos.x = 820;
			state = State::DashEnd;
		}
	}

}

void Boss::DashEnd()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	image = ImageManager::GetInstance()->FindImage("Dashend");
	if (timer > 0.1f)
	{

		frameIndex++;
		timer = 0;	
		
		if (frameIndex >= image->GetMaxFrameX())
		{
			frameIndex = 0;
			state = State::Idle;
		}
	}


}

void Boss::BulletAttack()
{
	//isFlip = true;
	image = ImageManager::GetInstance()->FindImage("Shoot");
	timer += TimerManager::GetInstance()->GetDeltaTime();
		if (timer > 0.1f)
		{
			frameIndex++;

			pos.x += cos(DEG_TO_RAD(angle-70)) * 30;
			pos.y += sin(DEG_TO_RAD(angle-70)) * 30;
			angle -= 2;

			timer = 0;

			if (frameIndex >= image->GetMaxFrameX())
			{
				frameIndex = image->GetMaxFrameX();
			}
			if (pos.y >= 360)
			{
				pos.y = 360;
				angle = 225;
				frameIndex = 0;
				//isFlip = false;
				state = State::TakeOutRifle;
			}
		}



	//state = State::TakeOutRifle; // 확률 넣기 rifle 또는 bomb
}

void Boss::Faint()
{
	image = ImageManager::GetInstance()->FindImage("Faint");
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > 0.1f)
	{
		frameIndex++;
		pos.x -= 3;
		timer = 0;

		if (frameIndex >= image->GetMaxFrameX())
		{
			frameIndex = -1;
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

void Boss::CheckLeft()
{
	if (isLeft) // 왼쪽
	{
		wallPos = {230,225};
	}
	else // 오른쪽
	{
		wallPos = {840,225};
	}
}

void Boss::CheckPlayerPos()
{
	if (pos.y >= 360) // 땅에 있을 때 기준으로 전환
	{
		if (pos.x < playerPos.x)
		{
			isFlip = false;
			isLeft = true;

		}
		else
		{
			isFlip = true;
			isLeft = false;
		}
	}
}


