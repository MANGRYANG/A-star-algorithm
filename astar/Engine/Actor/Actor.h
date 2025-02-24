#pragma once

#include "CoreMinimal.h"
#include "RTTI.h"
#include "Math/Vector2.h"

// Primary object of the level
class ENGINE_API Actor : public RTTI
{
	// RTTI declarations
	RTTI_DECLARATIONS(Actor, RTTI)

	// Declared Level Class as friend
	friend class Level;

public:
	Actor(Vector2 position);
	virtual ~Actor();

	// Function for loop processing
	virtual void Update(float deltaTime);
	virtual void Draw();

	// Getter & Setter
	virtual void SetPosition(const Vector2& newPosition);
	virtual Vector2 GetPosition() const;

	// Inline function
	inline bool IsActive() const { return isActive; }
	inline void SetActive(bool active) { isActive = active; }
	inline void Destroy() { isExpired = true; }

protected:
	// The location of the actor
	Vector2 position;

	// Variables indicating whether they are active
	bool isActive;

	// Variables that indicate whether the removal of the actor has been requested
	bool isExpired;
};