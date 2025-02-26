#include "Raft.h"

Raft::Raft(int length, Vector2 location) : Super("", location)
{
	std::string raft = "";
	for (int idx = 0; idx < length; ++idx)
	{
		raft += '*';
	}
	SetImage(raft.c_str());
	SetColor(Color::Yellow);
}

Raft::~Raft()
{
}
