// COMP710 GP 2D Framework 2017

// This include:
#include "enemybullet.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"

// Library includes:
#include <cassert>
#include <Windows.h>
#include <XInput.h>
#include <iostream>

EnemyBullet::EnemyBullet()
: Entity()
{

}

EnemyBullet::~EnemyBullet()
{

}

bool
EnemyBullet::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	return true;
}

void
EnemyBullet::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);

	// Horizontal boundary checking and position capping.
	if (m_x < 0 || m_x > Game::WIDTH_OF_PLAYING_PANEL)
	{
		m_dead = true;
	}

	// Vertical boundary checking and position capping.
	if (m_y > Game::HEIGHT_OF_PLAYING_PANEL)
	{
		m_dead = true;
	}
}