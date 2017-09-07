// COMP710 GP 2D Framework 2017

// This include:
#include "playership.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"

class Game;

// Library includes:
#include <cassert>
#include <Windows.h>
#include <XInput.h>
#include <iostream>

// Ensure the constructor chains-up to the super class constructor.
PlayerShip::PlayerShip()
: Entity()
{

}

PlayerShip::~PlayerShip()
{
	Entity::~Entity();
}

void
PlayerShip::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	// Dong: Set the initial position(384, 568) for the playership.
	float positionX = (Game::m_widthOfWindow - m_pSprite->GetWidth()) / 2.0f;
	float positionY = (Game::m_heightOfWindow - m_pSprite->GetHeight()) * 1.0f;

	this->SetPosition(positionX, positionY);
}

void
PlayerShip::Process(float deltaTime)
{
	Entity::Process(deltaTime);

	// Horizontal boundary checking and position capping.
	if (m_x <= 0)
	{
		m_x = 0;
	}
	else if (m_x >= Game::m_widthOfWindow - m_pSprite->GetWidth())
	{
		m_x = (Game::m_widthOfWindow - m_pSprite->GetWidth()) * 1.0f;
	}

	// Vertical boundary checking and position capping.
	if (m_y <= 0)
	{
		m_y = 0;
	}
	else if (m_y >= Game::m_heightOfWindow - m_pSprite->GetHeight())
	{
		m_y = (Game::m_heightOfWindow - m_pSprite->GetHeight()) * 1.0f;
	}
}