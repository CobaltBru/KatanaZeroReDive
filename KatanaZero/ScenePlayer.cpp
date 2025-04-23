#include "ScenePlayer.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "Image.h"
#include "ScrollManager.h"

ScenePlayer::ScenePlayer()
	:ObjectCollider(nullptr), ObjectRigidBody(nullptr), Speed(0.f)
{
}

HRESULT ScenePlayer::Init()
{
	float frameSpeed = 0.1f;
	aniIdx = 6;
	float scale = ScrollManager::GetInstance()->GetScale();
	Image* tmp;
	//¾É¾ÆÀÖÀ½
	tmp = ImageManager::GetInstance()->AddImage("talk_player_sit", L"Image/TalkScene/Player/spr_sitting.bmp", 32, 32, 1, 1, true, RGB(255, 0, 255));
	ani[0].Init(tmp, 1,2.5f);
	ani[0].setPos(Pos, false, true);
	ani[0].setAniTask({
		{0,10.f}
		});
	//idle
	tmp = ImageManager::GetInstance()->AddImage("talk_player_idle", L"Image/TalkScene/Player/spr_casual_idle.bmp", 330, 35, 11, 1, true, RGB(255, 0, 255));
	ani[1].Init(tmp, 11, 2.5f);
	ani[1].setPos(Pos, false, true);
	ani[1].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed}
		});
	//°È±â
	tmp = ImageManager::GetInstance()->AddImage("talk_player_walk", L"Image/TalkScene/Player/spr_casualwalk.bmp", 272, 36, 8, 1, true, RGB(255, 0, 255));
	ani[2].Init(tmp, 8, 2.5f);
	ani[2].setPos(Pos, false, true);
	ani[2].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed}
		});
	//ÁÖ»ç
	tmp = ImageManager::GetInstance()->AddImage("talk_player_getch", L"Image/TalkScene/Player/spr_player_get_chronos.bmp", 210, 32, 7, 1, true, RGB(255, 0, 255));
	ani[3].Init(tmp, 7, 2.5f);
	ani[3].setPos(Pos, false, true);
	ani[3].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed}
		});
	ani[3].setloopFlag(false);
	//ÁÖ»çº¹±Í
	tmp = ImageManager::GetInstance()->AddImage("talk_player_getch_rt", L"Image/TalkScene/Player/spr_player_get_chronos_return.bmp", 210, 32, 7, 1, true, RGB(255, 0, 255));
	ani[4].Init(tmp, 7, 2.5f);
	ani[4].setPos(Pos, false, true);
	ani[4].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed}
		});
	ani[4].setloopFlag(false);
	//¾É±â
	tmp = ImageManager::GetInstance()->AddImage("talk_player_sitdown", L"Image/TalkScene/Player/spr_sitdown.bmp", 384, 36, 12, 1, true, RGB(255, 0, 255));
	ani[5].Init(tmp, 12, 2.5f);
	ani[5].setPos(Pos, false, true);
	ani[5].setAniTask({
		{0,frameSpeed},{1,frameSpeed},{2,frameSpeed},
		{3,frameSpeed},{4,frameSpeed},{5,frameSpeed},
		{6,frameSpeed},{7,frameSpeed},{8,frameSpeed},
		{9,frameSpeed},{10,frameSpeed},{11,frameSpeed}
		});
	ani[5].setloopFlag(false);

	ani[6].Init(tmp, 12, 2.5f);
	ani[6].setPos(Pos, false, true);
	ani[6].setAniTask({
		{11,frameSpeed},{10,frameSpeed},{9,frameSpeed},
		{8,frameSpeed},{7,frameSpeed},{6,frameSpeed},
		{5,frameSpeed},{4,frameSpeed},{3,frameSpeed},
		{2,frameSpeed},{1,frameSpeed},{0,frameSpeed}
		});
	//ani[6].setloopFlag(false);
	ani[aniIdx].On();
	return S_OK;
}

void ScenePlayer::Release()
{
	for (int i = 0; i < 7; i++)
	{
		ani[i].Release();
	}
}

void ScenePlayer::Update()
{


	ani[aniIdx].Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void ScenePlayer::Render(HDC hdc)
{
	ani[aniIdx].Render(hdc);
}
