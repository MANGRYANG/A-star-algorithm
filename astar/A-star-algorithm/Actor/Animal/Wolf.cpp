#include "Wolf.h"

Wolf::Wolf(Vector2 location) : Super(" Wolf ", location)
{
	this->position.xpos = location.xpos;
	this->position.ypos = location.ypos;
	SetColor(Color::White);
}

Wolf::~Wolf()
{
}
