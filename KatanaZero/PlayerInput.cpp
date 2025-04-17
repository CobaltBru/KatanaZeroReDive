#include "PlayerInput.h"


void PlayerInput::Init()
{
	keyActionMap['A'] = EInputAction::Left;
	keyActionMap['D'] = EInputAction::Right;
	keyActionMap['W'] = EInputAction::Jump;
	keyActionMap['S'] = EInputAction::Down;

	keyActionMap[VK_LBUTTON] = EInputAction::Attack;	
}

// 무슨 키를 새로 눌렀는지
std::vector<EInputAction> PlayerInput::GetActions()
{
	std::vector<EInputAction> actions;
	for (const auto& keyAction : keyActionMap)
	{
		if (currKeyState[keyAction.first] && !prevKeyState[keyAction.first])	// Pressed
			actions.push_back(keyAction.second);
	}
	return actions;
}

// 키 State 업데이트
void PlayerInput::UpdateKeystate()
{
	prevKeyState = currKeyState;
	for (const auto& keyAction : keyActionMap)
	{
		currKeyState[keyAction.first] = KeyManager::GetInstance()->IsOnceKeyDown(keyAction.first);
	}
}
