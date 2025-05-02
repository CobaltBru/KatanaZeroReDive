#pragma once
#include "config.h"

class SpriteAnimation;
struct Animation
{
	SpriteAnimation* ani;
	string next;
};

class Animator
{
private:
	map<string, Animation> tree;

	FPOINT pos;
	bool flip;
	bool isEffect;

	Animation* currentAnimation;

public:
	void pushAnimation(string key, SpriteAnimation* ani, string next = "NONE");
	void Update(FPOINT pos, bool flip, bool isEffect);
	void Render(HDC hdc);
	void Release();

	void startAnimation(string key);
	Animation* FindAnimation(string key);
};

