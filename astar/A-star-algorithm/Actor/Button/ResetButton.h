#pragma once

#include "Engine/Engine.h"
#include "Button.h"

class ResetButton : public Button
{
	RTTI_DECLARATIONS(ResetButton, Button);

	ResetButton(Vector2 location);

	~ResetButton();
};