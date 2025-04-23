#include "ScenePsych.h"
#include "RenderManager.h"
#include "Image.h"


ScenePsych::ScenePsych()
{
	moveIdx = 0;
}

HRESULT ScenePsych::Init()
{
	moveEnd = false;
	float frameSpeed = 0.1f; 
	Image* tmp;
	//앉아있음
	tmp = ImageManager::GetInstance()->AddImage("talk_sit", L"Image/TalkScene/spr_psych_sitting.bmp", 55, 85, 1, 1, true, RGB(255, 0, 255));
	sit.Init(tmp, 1);
	sit.setPos(Pos, true, true);
	sit.setAniTask({
		{0,10.f}
		});
	sit.On();
	//일어나기
	tmp = ImageManager::GetInstance()->AddImage("talk_standUp", L"Image/TalkScene/spr_psych_standup.bmp", 845, 100, 13, 1, true, RGB(255, 0, 255));
	standUp.Init(tmp, 13);
	standUp.setPos(Pos, true, true);
	standUp.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed},{11,frameSpeed},
		{12,frameSpeed*3.f}
		}); 
	standUp.setloopFlag(false);

	//앉는동작
	sitDown.Init(tmp, 13);
	sitDown.setPos(Pos, true, true);
	sitDown.setAniTask({
		//{12,frameSpeed},{11,frameSpeed},{10,frameSpeed},
		{9,frameSpeed},{8,frameSpeed},{7,frameSpeed},
		{6,frameSpeed},{5,frameSpeed},{4,frameSpeed},
		{3,frameSpeed},{2,frameSpeed},{1,frameSpeed},
		{0,frameSpeed}
		});
	sitDown.setloopFlag(false);

	//걷기
	tmp = ImageManager::GetInstance()->AddImage("talk_walk", L"Image/TalkScene/spr_psych_walk.bmp", 520, 102, 10, 1, true, RGB(255, 0, 255));
	walk.Init(tmp, 10);
	walk.setPos(Pos, false, true);
	walk.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed}
		});
	walk.MoveOn({ 215.f,0.f }, 1.5f, Move_Linear | Move_Stop);


	//책상
	tmp = ImageManager::GetInstance()->AddImage("talk_desk", L"Image/TalkScene/spr_psych_grabdesk.bmp", 1240, 102, 20, 1, true, RGB(255, 0, 255));
	desk.Init(tmp, 20);
	desk.setPos(Pos, false, true);
	desk.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed},{11,frameSpeed},
		{12,frameSpeed},{13,frameSpeed},{14,frameSpeed},
		{15,frameSpeed},{16,frameSpeed},{17,frameSpeed},
		{18,frameSpeed*5.f}
		});
	desk.setloopFlag(false);
	//desk.MoveOn({ 215.f,0.f }, 1.5f, Move_Linear | Move_Stop | POS_Update);


	//주사기 들고걷기
	tmp = ImageManager::GetInstance()->AddImage("talk_walk_ch", L"Image/TalkScene/spr_psych_walk_syringe.bmp", 525, 102, 10, 1, true, RGB(255, 0, 255));
	walk_ch.Init(tmp, 10);
	walk_ch.setPos(Pos, true, true);
	walk_ch.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed}
		});
	walk_ch.MoveOn({ -215.f,0.f }, 1.5f, Move_Linear | Move_Stop);


	//주사하기
	tmp = ImageManager::GetInstance()->AddImage("talk_inject", L"Image/TalkScene/spr_psych_inject1.bmp", 2480, 102, 40, 1, true, RGB(255, 0, 255));
	inject.Init(tmp, 40);
	inject.setPos(Pos, true, true);
	inject.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},{3,frameSpeed},
		{4,frameSpeed},{5,frameSpeed},{6,frameSpeed},{7,frameSpeed},
		{8,frameSpeed},{9,frameSpeed},{10,frameSpeed},{11,frameSpeed},
		{12,frameSpeed},{13,frameSpeed},{14,frameSpeed},{15,frameSpeed},
		{16,frameSpeed},{17,frameSpeed},{18,frameSpeed},{19,frameSpeed},
		{20,frameSpeed},{21,frameSpeed},{22,frameSpeed},{23,frameSpeed},
		{24,frameSpeed},{25,frameSpeed},{26,frameSpeed},{27,frameSpeed*10.f},
		{28,frameSpeed*10.f},{29,frameSpeed},{30,frameSpeed},{31,frameSpeed},
		{32,frameSpeed},{33,frameSpeed},{34,frameSpeed},{35,frameSpeed},
		{36,frameSpeed},{37,frameSpeed},{38,frameSpeed},{39,frameSpeed}
		});
	inject.setloopFlag(false);

	moveIdx = 0;
	MoveList[0] = &sit;
	MoveList[1] = &standUp;
	MoveList[2] = &walk;
	MoveList[3] = &desk;
	MoveList[4] = &walk_ch;
	MoveList[5] = &inject;
	MoveList[6] = &walk_ch;
	MoveList[7] = &sitDown;
	return S_OK;
}

void ScenePsych::Release()
{
	sit.Release();
	standUp.Release();
	sitDown.Release();
	walk.Release();
	desk.Release();
	walk_ch.Release();
	inject.Release();
}

void ScenePsych::Update()
{
	/*if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		Move();
	}*/
	if (currentTask == TASK::MOVE)
	{
		MoveFunc();
	}
	
	MoveList[moveIdx]->Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void ScenePsych::Render(HDC hdc)
{
	MoveList[moveIdx]->Render(hdc);
}

void ScenePsych::Move()
{
	currentTask = TASK::MOVE;
	moveIdx = 1;
	MoveList[1]->setPos({ Pos.x,Pos.y}, true, true);
	MoveList[1]->On();
	
}

void ScenePsych::MoveFunc()
{
	if ((moveIdx == 1) && MoveList[moveIdx]->getAniComplete())
	{
		MoveList[moveIdx]->Off();
		moveIdx = 2;
		Pos = MoveList[moveIdx - 1]->getCurrentPos();
		MoveList[moveIdx]->setPos({ Pos.x,Pos.y - 7.f }, false, true);
		MoveList[moveIdx]->MoveOn({ 215.f,0.f }, 1.7f, Move_Linear | Move_Stop);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
	}
	else if ((moveIdx == 2) && MoveList[moveIdx]->getMoveComplete())
	{
		MoveList[moveIdx]->Off();
		moveIdx = 3;
		Pos = MoveList[moveIdx - 1]->getCurrentPos();
		MoveList[moveIdx]->setPos(Pos, false, true);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
	}
	else if ((moveIdx == 3) && MoveList[moveIdx]->getAniComplete())
	{
		MoveList[moveIdx]->Off();
		moveIdx = 4;
		Pos = MoveList[moveIdx - 1]->getCurrentPos();
		MoveList[moveIdx]->setPos(Pos, true, true);
		MoveList[moveIdx]->MoveOn({ -350.f,0.f }, 2.5f, Move_Linear | Move_Stop);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
	}
	else if ((moveIdx == 4) && MoveList[moveIdx]->getMoveComplete())
	{
		MoveList[moveIdx]->Off();
		moveIdx = 5;
		Pos = MoveList[moveIdx - 1]->getCurrentPos();
		MoveList[moveIdx]->setPos(Pos, true, true);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
	}
	else if ((moveIdx == 5) && MoveList[moveIdx]->getAniComplete())
	{
		MoveList[moveIdx]->Off();
		moveIdx = 6;
		Pos = MoveList[moveIdx -1]->getCurrentPos();
		MoveList[moveIdx]->setPos(Pos, false, true);
		MoveList[moveIdx]->MoveOn({ 135.f,0.f }, 1.5f, Move_Linear | Move_Stop);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
	}
	else if ((moveIdx == 6) && MoveList[moveIdx]->getMoveComplete())
	{
		MoveList[moveIdx]->Off();
		moveIdx = 7;
		Pos = MoveList[moveIdx - 1]->getCurrentPos();
		MoveList[moveIdx]->setPos(Pos, true, true);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
	}
	else if ((moveIdx == 7) && MoveList[moveIdx]->getAniComplete())
	{
		MoveList[moveIdx]->Off();
		Pos = MoveList[moveIdx]->getCurrentPos();
		moveIdx = 0;
		MoveList[moveIdx]->setPos({ Pos.x,Pos.y + 7.f }, true, true);
		MoveList[moveIdx]->Start();
		MoveList[moveIdx]->On();
		moveEnd = true;
	}
}
