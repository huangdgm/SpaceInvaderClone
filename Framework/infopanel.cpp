// COMP710 GP 2D Framework 2017

// This include:
#include "infopanel.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>
#include <Windows.h>
#include <SDL_ttf.h>

// Ensure the constructor chains-up to the super class constructor.
InfoPanel::InfoPanel()
: Entity()
{

}

InfoPanel::~InfoPanel()
{

}

bool
InfoPanel::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
	Entity::SetPosition(Game::WIDTH_OF_PLAYING_PANEL * 1.0f, 0);

	m_dead = false;

	return true;
}

void
InfoPanel::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);

	// dong: no need to perform boundary check, as the position doesn't change
}