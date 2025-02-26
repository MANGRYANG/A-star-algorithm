#include "Chick.h"

Chick::Chick(Vector2 location) : Super(" Chick ", location)
{
	this->position.xpos = location.xpos;
	this->position.ypos = location.ypos;
	SetColor(Color::Bright_Yellow);
}

Chick::~Chick()
{
}
