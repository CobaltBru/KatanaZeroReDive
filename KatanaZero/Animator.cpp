#include "Animator.h"
#include "SpriteAnimation.h"

void Animator::pushAnimation(string key, SpriteAnimation* ani, string next)
{
	SAnimation tmp = { ani, next };
	tree.insert({ key,tmp });
}

void Animator::Update(FPOINT pos, bool flip, bool isEffect)
{
	this->pos = pos;
	this->flip = flip;
	this->isEffect = isEffect;

	if (currentAnimation->next != "NONE")
	{
		if (currentAnimation->ani->isFinish())
		{
			currentAnimation = FindAnimation(currentAnimation->next);
			currentAnimation->ani->reset();
		}
	}
	currentAnimation->ani->Update(pos, flip, isEffect);
}

void Animator::Render(HDC hdc)
{
	currentAnimation->ani->Render(hdc);
}

void Animator::Release()
{
	for (auto& node : tree)
	{
		if (node.second.ani != nullptr)
		{
			delete node.second.ani;
			node.second.ani = nullptr;
		}
	}
	tree.clear();
}

void Animator::startAnimation(string key)
{
	currentAnimation = FindAnimation(key);
	currentAnimation->ani->reset();
}

SAnimation* Animator::FindAnimation(string key)
{
	auto it = tree.find(key);
	if (it != tree.end())
	{
		FindAnimKey(key);
		return &(it->second);
	}
	else
	{
		return nullptr;
	}
}

GPImage* Animator::getGPImage()
{
	return currentAnimation->ani->getGPImage();
}

Image* Animator::getImage()
{
	return currentAnimation->ani->getImage();
}

int Animator::getCurrFrame()
{
	return currentAnimation->ani->getCurrFrame();
}

void Animator::FindAnimKey(string key)
{
	if (key == "idle") currAnimKey = "dragon_idle";
	if (key == "run") currAnimKey = "dragon_run";
	if (key == "idle_to_run") currAnimKey = "dragon_idle_to_run";
	if (key == "run_to_idle") currAnimKey = "dragon_run_to_idle";
	if (key == "jump") currAnimKey = "dragon_jump";
	if (key == "fall") currAnimKey = "dragon_fall";
	if (key == "roll") currAnimKey = "dragon_roll";
	if (key == "crouch") currAnimKey = "dragon_crouch";
	if (key == "attack") currAnimKey = "dragon_attack";
	if (key == "hurtfly") currAnimKey = "dragon_hurtfly";
	if (key == "hurtground") currAnimKey = "dragon_hurtground";
}
