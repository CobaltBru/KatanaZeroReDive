#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"
//�ɱ�
//�Ͼ��->������ �ȱ�
//�ನ����
//���� ���� �ȱ�
//�����
//������ ���� �ȱ�
//�ɴ���->�ɱ�

enum TASK{SIT,MOVE};

class Image;

class Animation;
class ScenePsych : public GameObject
{
private:
	Animation sit;
	Animation standUp;
	Animation sitDown;
	Animation walk;
	Animation desk;
	Animation walk_ch;
	Animation inject;
	TASK currentTask;

	float timer;
public:
	ScenePsych();
	virtual ~ScenePsych() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;



};

