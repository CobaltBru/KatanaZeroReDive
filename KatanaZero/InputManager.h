#pragma once
#include <functional>
#include "Singleton.h"

// ����: ���ϴ� state map�� state key�� function ���ε�

class InputManager: public Singleton<InputManager>
{
	typedef std::function<void()> stateFunction;

	void RegisterState() {};

};

