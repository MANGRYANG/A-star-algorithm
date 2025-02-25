#include "Text.h"
#include "Engine/Engine.h"
#include "Math/Vector2.h"

Text::Text(const char* textMessage, Vector2 location)
	: Super(textMessage, location)
{
	this->position.xpos = location.xpos;
	this->position.ypos = location.ypos;
}

Text::Text(const char* textMessage, Vector2 location, Color color)
	: Super(textMessage, location)
{
	this->position.xpos = location.xpos;
	this->position.ypos = location.ypos;
	this->color = color;
}

Text::~Text()
{
}

void Text::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Text::SetText(const char* newText)
{
	Super::SetImage(newText);
}

void Text::SetPosition(const Vector2& newPosition)
{
	Super::SetPosition(newPosition);
}

void Text::SetColor(Color color)
{
	Super::SetColor(color);
}
