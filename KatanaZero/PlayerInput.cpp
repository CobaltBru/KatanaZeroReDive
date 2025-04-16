#include "PlayerInput.h"

void PlayerInput::Init()
{
	keyActionMap['A'] = InputAction::Left;
	keyActionMap['D'] = InputAction::Right;
	keyActionMap['W'] = InputAction::Jump;
	keyActionMap['S'] = InputAction::Down;

	keyActionMap['P'] = InputAction::Attack;
	
}

void PlayerInput::BindKeyAction()
{
	
}
