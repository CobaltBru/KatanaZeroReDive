#pragma once
#include <functional>
#include "Singleton.h"
#include "config.h"

// ����: ���ϴ� EState map�� EState key�� function ���ε�

class InputManager: public Singleton<InputManager>
{
	typedef std::function<void()> stateFunction;

	void RegisterState() {};

};

