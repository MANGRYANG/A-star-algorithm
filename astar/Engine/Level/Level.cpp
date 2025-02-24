#include "PCH.h"
#include "Level.h"
#include "Actor/Actor.h"

Level::Level()
{
}

Level::~Level()
{
	for (Actor* actor : actors)
	{
		delete actor;
	}
}

void Level::AddActor(Actor* newActor)
{
	addRequestedActors.PushBack(newActor);
}

void Level::ProcessAddedAndDestroyedActor()
{
	// Processed the requested actor after the actor cycle
	for (int ix = 0; ix < actors.Size();)
	{
		if (actors[ix]->isExpired)
		{
			delete actors[ix];
			actors[ix] = nullptr;
			actors.Erase(ix);
			continue;
		}

		++ix;
	}

	// Additional requested actor processing
	for (Actor* actor : addRequestedActors)
	{
		actors.PushBack(actor);
	}
	addRequestedActors.Clear();

}

void Level::Update(float deltaTime)
{
	// Call the Update function while traversing the actors included in the level
	for (Actor* actor : actors)
	{
		// Skip if the actor is inactive or requested to be deleted
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		actor->Update(deltaTime);
	}
}

void Level::Render()
{
	int idx = 0;
	// Call the Draw function while traversing the actors included in the level
	for (Actor* actor : actors)
	{
		// Skip if the actor is inactive or requested to be deleted
		if (actor->isExpired)
		{
			actors.Erase(idx);
		}
		if (!actor->isActive)
		{
			++idx;
			continue;
		}

		actor->Draw();
		++idx;
	}
}