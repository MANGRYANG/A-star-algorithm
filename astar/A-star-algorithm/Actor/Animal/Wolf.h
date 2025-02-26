#pragma once

#include "Engine/Engine.h"
#include "Actor/DrawableActor.h"

class Wolf : public DrawableActor
{
	RTTI_DECLARATIONS(Wolf, DrawableActor);

	Wolf(Vector2 location);

	~Wolf();
};