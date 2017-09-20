// COMP710 GP 2D Framework 2017

// The background.wav music is owned by Jan125 from:
// https://opengameart.org/content/stereotypical-90s-space-shooter-music
// The boss sprite is owned by Skorpio from:
// https://opengameart.org/content/spaceship-fighter

// Library includes:
#include <SDL.h>
#include <crtdbg.h>
#include <ctime>

// Local includes:
#include "game.h"
#include "logmanager.h"

int main(int argc, char* argv[])
{
	// detect memory leak
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand((unsigned int)time(0));

	Game& gameInstance = Game::GetInstance();

	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game initialise failed!");

		return (1);
	}
	
	while (Game::sm_gameState != GAME_QUIT)
	{
		gameInstance.DoGameLoop();
	}

	Game::DestroyInstance();
	LogManager::DestroyInstance();

	return (0);
}
