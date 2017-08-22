// COMP710 GP 2D Framework 2017

// This include:
#include "enemy.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <Windows.h>

// Ensure the constructor chains-up to the super class constructor.
Enemy::Enemy()
: Entity()
{

}

Enemy::~Enemy()
{

}

void
Enemy::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);

	// dong: no need to perform boundary check, as the position doesn't change
}