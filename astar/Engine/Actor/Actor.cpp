#include "PCH.h"
#include "Actor.h"

#include "CoreMinimal.h"

Actor::Actor(Vector2 position)
	: isActive(true), isExpired(false)
{
}

Actor::~Actor()
{
}

void Actor::Update(float deltaTime) 
{
}

void Actor::Draw() 
{
}

void Actor::SetPosition(const Vector2& newPosition)
{
	position = newPosition;
}

Vector2 Actor::GetPosition() const
{
	return position;
}