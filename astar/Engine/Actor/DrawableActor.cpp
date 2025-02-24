#include "PCH.h"
#include "Engine/Engine.h"
#include "DrawableActor.h"

DrawableActor::DrawableActor(const char* image, Vector2 position)
	: Actor(position)
{
	auto length = strlen(image) + 1;
	this->image = new char[length];
	strcpy_s(this->image, length, image);

	width = (int)strlen(image);

	SetPosition(position);
}

DrawableActor::~DrawableActor()
{
	delete[] image;
}

void DrawableActor::Draw()
{
	Super::Draw();

	Engine::Get().Render(position, image, color);
}

void DrawableActor::SetImage(const char* newImage)
{
	auto length = strlen(newImage) + 1;
	this->image = new char[length];
	strcpy_s(this->image, length, newImage);

	width = (int)strlen(newImage);
}

void DrawableActor::SetPosition(const Vector2& newPosition)
{
	Super::SetPosition(newPosition);
}

Vector2 DrawableActor::GetPosition() const
{
	return Super::GetPosition();
}

void DrawableActor::SetColor(const Color& color)
{
	this->color = color;
}
