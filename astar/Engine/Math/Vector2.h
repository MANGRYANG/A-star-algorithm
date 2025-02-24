#pragma once

#include "CoreMinimal.h"

class ENGINE_API Vector2
{
public:
	// Constructor & Destructor
	Vector2(int xpos = 0, int ypos = 0);
	~Vector2() = default;

	// Operator overloading
	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	bool operator==(const Vector2& other);
	bool operator!=(const Vector2& other);

public:
	// 2D coordinates
	int xpos = 0;
	int ypos = 0;
};