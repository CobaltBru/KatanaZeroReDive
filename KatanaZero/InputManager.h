#pragma once
#include <functional>
#include "Singleton.h"

// 역할: 원하는 state map에 state key와 function 바인딩

class InputManager: public Singleton<InputManager>
{
	typedef std::function<void()> stateFunction;

	void RegisterState() {};

};

