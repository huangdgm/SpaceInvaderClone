// COMP710 GP 2D Framework 2017

// This include:
#include "gamesummary.h"

// Local includes:

// Library includes:


GameSummary::GameSummary()
: Entity()
{

}

GameSummary::~GameSummary()
{

}

void
GameSummary::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	this->SetPosition(0, 0);
}
