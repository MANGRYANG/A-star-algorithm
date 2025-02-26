#pragma once

#include "Engine/Engine.h"
#include "Actor/DrawableActor.h"

class Chick : public DrawableActor
{
	RTTI_DECLARATIONS(Chick, DrawableActor);

	Chick(Vector2 location);

	~Chick();
};