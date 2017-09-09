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
, m_score(0)
, m_health(0)
, m_numOfLivesLeft(0)
{

}

PlayerShip::~PlayerShip()
{

}

void
PlayerShip::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;
	m_health = 100;
	m_numOfLivesLeft = 2;

	// Dong: Set the initial position(384, 568) for the playership.
	float positionX = (Game::WIDTH_OF_WINDOW - m_pSprite->GetWidth()) / 2.0f;
	float positionY = (Game::HEIGHT_OF_WINDOW - m_pSprite->GetHeight()) * 1.0f;

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
	else if (m_x >= Game::WIDTH_OF_PLAYING_PANEL - m_pSprite->GetWidth())
	{
		m_x = (Game::WIDTH_OF_PLAYING_PANEL - m_pSprite->GetWidth()) * 1.0f;
	}

	// Vertical boundary checking and position capping.
	if (m_y <= 0)
	{
		m_y = 0;
	}
	else if (m_y >= Game::HEIGHT_OF_PLAYING_PANEL - m_pSprite->GetHeight())
	{
		m_y = (Game::HEIGHT_OF_PLAYING_PANEL - m_pSprite->GetHeight()) * 1.0f;
	}
}

void
PlayerShip::SetScore(int score)
{
	m_score = score;
}

void
PlayerShip::SetHealth(int health)
{
	m_health = health;
}

void
PlayerShip::SetNumOfLivesLeft(int numOfLivesLeft)
{
	m_numOfLivesLeft = numOfLivesLeft;
}

int 
PlayerShip::GetScore() const
{
	return m_score;
}

int
PlayerShip::GetHealth() const
{
	return m_health;
}

int
PlayerShip::GetNumOfLivesLeft() const
{
	return m_numOfLivesLeft;
}