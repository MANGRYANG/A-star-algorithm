#include "Animal.h"

Animal::Animal(const char* name, Vector2 location, Color color) : Super(name, location)
{
	SetColor(color);
}

Animal::~Animal()
{
}
