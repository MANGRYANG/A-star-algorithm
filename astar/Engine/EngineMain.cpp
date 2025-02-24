#include "PCH.h"
#include "Engine/Engine.h"

int main(void)
{
	// Check memory leak
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;
	engine.Run();
}