#pragma once

#include "CoreMinimal.h"
#include "RTTI.h"
#include "Container/List.h"

// Forward Declaration
class Actor;
class ENGINE_API Level : public RTTI
{
	// RTTI declarations
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// Functions for adding actor
	void AddActor(Actor* newActor);

	// Function that cleans up the actors requested to be deleted
	// void DestroyActor();

	// Function for adding and deleting actors
	void ProcessAddedAndDestroyedActor();

	// Function for loop processing
	virtual void Update(float deltaTime);
	virtual void Render();

protected:
	// The array of actors placed in the gamespace
	List<Actor*> actors;

	// Additional requested actors
	List<Actor*> addRequestedActors;
};