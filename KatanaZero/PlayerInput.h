#pragma once
#include <unordered_map>
#include "config.h"
#include <functional>
#include <cstddef>
#include <vector>

class SpriteAnimator;

enum class EInputAction
{
	Idle,
	Left,
	Right,
	Dash,
	Down,
	Jump,
	Fall,
	Attack,
	SlowTime,
	Dead,
};

struct KeyInfo {
	UINT virtualKey;
	EInputAction action;
	std::function<void()> func;	
	
	Image* image;
	SpriteAnimator* animator;

	int FrameIndex;
	int MaxFrameIndex;
};

// unordered_map에서 사용하기 위해 hash function 제공
namespace std {
	template <>
	struct hash<EInputAction> {
		size_t operator()(const EInputAction& action) const {
			return static_cast<size_t>(action);
		}
	};
}

class PlayerInput
{
private:
	std::unordered_map<UINT, EInputAction> keyActionMap;	

	bitset<MAX_KEY_COUNT> currKeyState;
	bitset<MAX_KEY_COUNT> prevKeyState;

public:
	void Init();
	
	std::unordered_map<UINT, EInputAction>& GetkeyActionMap() { return keyActionMap; };

	std::vector<EInputAction> GetActions();

	void UpdateKeystate();
};

