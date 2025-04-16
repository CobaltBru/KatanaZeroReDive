#pragma once
#include <unordered_map>
#include "config.h"
#include <functional>
#include <cstddef>
#include <vector>


enum class InputAction
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

// unordered_map에서 사용하기 위해 hash function 제공
namespace std {
	template <>
	struct hash<InputAction> {
		size_t operator()(const InputAction& action) const {
			return static_cast<size_t>(action);
		}
	};
}

class PlayerInput
{
private:
	std::unordered_map<UINT, InputAction> keyActionMap;	

public:
	void Init();
	
	std::unordered_map<UINT, InputAction>& GetkeyActionMap() { return keyActionMap; };

	std::vector<InputAction> GetActions();

	void BindKeyAction();
};

