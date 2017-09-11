// COMP710 GP 2D Framework 2017

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>
#include <string>

using namespace std;

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool
InputHandler::Initialise()
{
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
	{
		LogManager::GetInstance().Log("Joystick could not initialize!");
	}

	int numController = SDL_NumJoysticks();

	// Give warning if no joysticks connected.
	if (numController < 1)
	{
		LogManager::GetInstance().Log("Warning: no joysticks connected!");
	}

	m_pGameController = SDL_JoystickOpen(0);	// Open the joystick of device_index 0

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void
InputHandler::ProcessInputFromPlayGame(Game& game)
{
	SDL_Event e;

	// Loop until there are no events left in the event queue.
	// The SDL_PollEvent() function takes a pointer to an SDL_Event structure that is to be filled with event information.
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			game.Quit();

			break;
		case SDL_JOYBUTTONDOWN:
			// Tell the game to fire a player bullet...
			if (e.jbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				game.FireSpaceShipBullet();
			}

			break;
		case SDL_JOYHATMOTION:
			if (e.jhat.value == SDL_HAT_LEFT)
			{
				game.MoveSpaceShipLeft();
			}
			else if (e.jhat.value == SDL_HAT_RIGHT)
			{
				game.MoveSpaceShipRight();
			}
			else if (e.jhat.value == SDL_HAT_UP)
			{
				game.MoveSpaceShipUp();
			}
			else if (e.jhat.value == SDL_HAT_DOWN)
			{
				game.MoveSpaceShipDown();
			}
			else if (e.jhat.value == SDL_HAT_CENTERED)
			{
				game.StopSpaceShip();
			}

			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_RIGHT)
			{
				game.MoveSpaceShipRight();
			}
			else if (e.key.keysym.sym == SDLK_LEFT)
			{
				game.MoveSpaceShipLeft();
			}
			else if (e.key.keysym.sym == SDLK_UP)
			{
				game.MoveSpaceShipUp();
			}
			else if (e.key.keysym.sym == SDLK_DOWN)
			{
				game.MoveSpaceShipDown();
			}
			else if (e.key.keysym.sym == SDLK_SPACE)
			{
				game.FireSpaceShipBullet();
			}

			break;
		case SDL_KEYUP:
			game.StopSpaceShip();

			break;
		}
	}
}

void
InputHandler::ProcessInputFromSplashScreen(SplashScreen& splashScreen)
{
	SDL_Event e;

	// Loop until there are no events left in the event queue.
	// The SDL_PollEvent() function takes a pointer to an SDL_Event structure that is to be filled with event information.
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			//game.Quit();

			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_KEYDOWN:
			break;
		}
	}
}
