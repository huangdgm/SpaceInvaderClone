// COMP710 GP 2D Framework 2017

// This include:
#include "playerbullet.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <Windows.h>
#include <XInput.h>
#include <iostream>

PlayerBullet::PlayerBullet()
: Entity()
{

}

PlayerBullet::~PlayerBullet()
{

}

bool
PlayerBullet::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	return true;
}

void
PlayerBullet::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);
}