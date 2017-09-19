// COMP710 GP 2D Framework 2017

// This include:
#include "splashscreen.h"

// Local includes:

// Library includes:


SplashScreen::SplashScreen()
: Entity()
{

}

SplashScreen::~SplashScreen()
{

}

void
SplashScreen::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	this->SetPosition(0, 0);
}

void
SplashScreen::SetSprite(Sprite* sprite)
{
	Entity::Initialise(sprite);
}
