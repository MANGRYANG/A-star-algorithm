#include "Game/Game.h"

int main(void)
{
	Game game;
	game.LoadLevel(new GameplayLevel());
	game.Run();
	return 0;
}