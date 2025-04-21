#pragma once
#include <functional>
#include "Singleton.h"
#include "config.h"

// 역할: 원하는 EState map에 EState key와 function 바인딩

class InputManager: public Singleton<InputManager>
{
	typedef std::function<void()> stateFunction;

	void RegisterState() {};

};

