#include "ScenePsych.h"
#include "RenderManager.h"
#include "Image.h"


ScenePsych::ScenePsych()
{
}

HRESULT ScenePsych::Init()
{
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
		{12,frameSpeed}
		}); 
	standUp.setloopFlag(false);
	standUp.On();

	//앉는동작
	sitDown.Init(tmp, 13);
	sitDown.setPos(Pos, true, true);
	sitDown.setAniTask({
		{12,frameSpeed},{11,frameSpeed},{10,frameSpeed},
		{9,frameSpeed},{8,frameSpeed},{7,frameSpeed},
		{6,frameSpeed},{5,frameSpeed},{4,frameSpeed},
		{3,frameSpeed},{2,frameSpeed},{1,frameSpeed},
		{0,frameSpeed}
		});
	sitDown.setloopFlag(false);
	sitDown.On();

	//걷기
	tmp = ImageManager::GetInstance()->AddImage("talk_walk", L"Image/TalkScene/spr_psych_walk.bmp", 525, 102, 10, 1, true, RGB(255, 0, 255));
	walk.Init(tmp, 10);
	walk.setPos(Pos, true, true);
	walk.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed}
		});
	walk.MoveOn({ 215.f,0.f }, 1.5f, Move_Linear | Move_Stop | POS_Update);
	walk.On();

	//책상
	tmp = ImageManager::GetInstance()->AddImage("talk_desk", L"Image/TalkScene/spr_psych_grabdesk.bmp", 1187, 102, 19, 1, true, RGB(255, 0, 255));
	desk.Init(tmp, 19);
	desk.setPos(Pos, true, true);
	desk.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed},{11,frameSpeed},
		{12,frameSpeed},{13,frameSpeed},{14,frameSpeed},
		{15,frameSpeed},{16,frameSpeed},{17,frameSpeed},
		{18,frameSpeed}
		});
	desk.setloopFlag(false);
	//desk.MoveOn({ 215.f,0.f }, 1.5f, Move_Linear | Move_Stop | POS_Update);
	desk.On();

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
	walk_ch.MoveOn({ -215.f,0.f }, 1.5f, Move_Linear | Move_Stop | POS_Update);
	walk_ch.On();

	//주사하기
	tmp = ImageManager::GetInstance()->AddImage("talk_inject", L"Image/TalkScene/spr_psych_inject.bmp", 1000 * 2.5f, 41 * 2.5f, 40, 1, true, RGB(255, 0, 255));
	inject.Init(tmp, 40);
	inject.setPos(Pos, true, true);
	inject.setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},{3,frameSpeed},
		{4,frameSpeed},{5,frameSpeed},{6,frameSpeed},{7,frameSpeed},
		{8,frameSpeed},{9,frameSpeed},{10,frameSpeed},{11,frameSpeed},
		{12,frameSpeed},{13,frameSpeed},{14,frameSpeed},{15,frameSpeed},
		{16,frameSpeed},{17,frameSpeed},{18,frameSpeed},{19,frameSpeed},
		{20,frameSpeed},{21,frameSpeed},{22,frameSpeed},{23,frameSpeed},
		{24,frameSpeed},{25,frameSpeed},{26,frameSpeed},{27,frameSpeed},
		{28,frameSpeed},{29,frameSpeed},{30,frameSpeed},{31,frameSpeed},
		{32,frameSpeed},{33,frameSpeed},{34,frameSpeed},{35,frameSpeed},
		{36,frameSpeed},{37,frameSpeed},{38,frameSpeed},{39,frameSpeed}
		});
	inject.On();
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
	inject.Update();
	inject.setPos(Pos,true,true);

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void ScenePsych::Render(HDC hdc)
{
	sit.Render(hdc);

}
