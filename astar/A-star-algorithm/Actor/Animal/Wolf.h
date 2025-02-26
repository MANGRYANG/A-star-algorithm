#pragma once

#include "Engine/Engine.h"
#include "Animal.h"

class Wolf : public Animal
{
	RTTI_DECLARATIONS(Wolf, Animal);

	Wolf(Vector2 location);

	~Wolf();
};