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
