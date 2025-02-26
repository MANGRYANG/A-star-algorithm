#pragma once

#include "Engine/Engine.h"
#include "Actor/DrawableActor.h"

class Animal : public DrawableActor
{
	RTTI_DECLARATIONS(Animal, DrawableActor);

	Animal(const char* name, Vector2 location, Color color);

	~Animal();
};