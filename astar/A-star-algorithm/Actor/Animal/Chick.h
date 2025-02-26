#pragma once

#include "Engine/Engine.h"
#include "Animal.h"

class Chick : public Animal
{
	RTTI_DECLARATIONS(Chick, Animal);

	Chick(Vector2 location);

	~Chick();
};