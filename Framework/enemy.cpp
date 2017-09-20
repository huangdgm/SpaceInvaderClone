// COMP710 GP 2D Framework 2017

// This include:
#include "enemy.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"

// Library includes:

Enemy::Enemy()
: Entity()
{

}

Enemy::~Enemy()
{

}

bool
Enemy::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	return true;
}

void
Enemy::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);

	// Horizontal boundary checking and position capping.
	if (m_x < m_pSprite->GetWidth() * (-1.0f) || m_x > Game::WIDTH_OF_PLAYING_PANEL)
	{
		m_dead = true;
	}

	// Vertical boundary checking and position capping.
	if (m_y > Game::HEIGHT_OF_PLAYING_PANEL)
	{
		m_dead = true;
	}
}