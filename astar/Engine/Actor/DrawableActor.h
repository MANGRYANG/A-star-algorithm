#pragma once

#include "Actor.h"

class ENGINE_API DrawableActor : public Actor
{
	// RTTI.
	RTTI_DECLARATIONS(DrawableActor, Actor)

public:
	DrawableActor(const char* image, Vector2 position);
	virtual ~DrawableActor();

	virtual void Draw() override;

	void SetImage(const char* newImage);

	virtual void SetPosition(const Vector2& newPosition) override;
	Vector2 GetPosition() const;

	void SetColor(const Color& color);

	// Getter.
	inline int Width() const { return width; }

protected:

	char* image;

	int width = 0;

	Color color = Color::White;
};
