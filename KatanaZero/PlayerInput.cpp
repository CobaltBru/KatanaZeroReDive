#include "PlayerInput.h"


void PlayerInput::Init()
{
	keyActionMap['A'] = InputAction::Left;
	keyActionMap['D'] = InputAction::Right;
	keyActionMap['W'] = InputAction::Jump;
	keyActionMap['S'] = InputAction::Down;

	keyActionMap[VK_LBUTTON] = InputAction::Attack;
	
}

std::vector<InputAction> PlayerInput::GetActions()
{
	std::vector<InputAction> actions;
	for (const auto& keyAction : keyActionMap)
	{
		if (KeyManager::GetInstance()->IsStayKeyDown(keyAction.first))
			actions.push_back(keyAction.second);
	}
	return actions;
}

void PlayerInput::BindKeyAction()
{
	
}
