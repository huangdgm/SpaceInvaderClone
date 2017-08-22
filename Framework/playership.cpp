// COMP710 GP 2D Framework 2017

// This include:
#include "playership.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

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

}

void
PlayerShip::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
	this->SetPosition(384, 568);
}

void
PlayerShip::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);

	// W03.1: Generic position update, based upon velocity (and time).
	// dong: The playership only has velocity on x.
	m_x += m_velocityX * deltaTime;

	// W03.1: Boundary checking and position capping.
	// dong: Only need to check the boundary horizontally
	if (m_x <= 0)
	{
		m_x = 0;
	}
	else if (m_x >= 800 - 32)
	{
		m_x = 800 - 32;
	}
}