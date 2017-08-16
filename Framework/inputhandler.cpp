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
InputHandler::ProcessInput(Game& game)
{
	// W03.1: Receive Input Events below...
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);	// initialise subsystem

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
			// W03.3: Tell the game to fire a player bullet...
			if (e.jbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				game.FireSpaceShipBullet();
			}

			break;
		case SDL_JOYHATMOTION:
			if (e.jhat.value == 8)
			{
				game.MoveSpaceShipLeft();
			}
			else if (e.jhat.value == 2)
			{
				game.MoveSpaceShipRight();
			}
			else if (e.jhat.value == SDL_HAT_CENTERED)
			{
				game.StopSpaceShip();
			}

			break;
		}
	}
}
