#pragma once
#include "config.h"

class SpriteAnimation;
class GPImage;
class Image;
struct SAnimation
{
	SpriteAnimation* ani;
	string next;
};

class Animator
{
private:
	map<string, SAnimation> tree;

	FPOINT pos;
	bool flip;
	bool isEffect;

	SAnimation* currentAnimation;
	string currAnimKey;

public:
	void pushAnimation(string key, SpriteAnimation* ani, string next = "NONE");
	void Update(FPOINT pos, bool flip, bool isEffect);
	void Render(HDC hdc);
	void Release();

	void startAnimation(string key);
	SAnimation* FindAnimation(string key);
	GPImage* getGPImage();
	Image* getImage();
	int getCurrFrame();
	void FindAnimKey(string key);
	string getCurrAnimKey() { return currAnimKey; }
};

