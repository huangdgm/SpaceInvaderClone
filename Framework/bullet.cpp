// COMP710 GP 2D Framework 2017

// This include:
#include "bullet.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <Windows.h>
#include <XInput.h>
#include <iostream>

// Ensure the constructor chains-up to the super class constructor.
Bullet::Bullet()
: Entity()
{

}

Bullet::~Bullet()
{

}

bool
Bullet::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	return true;
}

void
Bullet::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);
}