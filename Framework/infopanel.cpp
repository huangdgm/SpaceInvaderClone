// COMP710 GP 2D Framework 2017

// This include:
#include "infopanel.h"

// Local includes:
#include "game.h"

// Library includes:

class Sprite;

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
	Entity::Process(deltaTime);
}