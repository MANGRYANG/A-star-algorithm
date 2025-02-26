#pragma once

#include "Engine/Engine.h"
#include "Actor/DrawableActor.h"

class Raft : public DrawableActor
{
	RTTI_DECLARATIONS(Raft, DrawableActor);

	Raft(int length, Vector2 location);

	~Raft();
};