#pragma once

#include "Engine/Engine.h"
#include "Actor/DrawableActor.h"

class Button : public DrawableActor
{
	RTTI_DECLARATIONS(Button, DrawableActor);

	Button(const char* text, Vector2 location);

	~Button();
};