// COMP710 GP 2D Framework 2017

// This include:
#include "playerbullet.h"

// Local includes:
#include "sprite.h"

// Library includes:

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
	Entity::Process(deltaTime);

	if (m_y <= m_pSprite->GetHeight() * (-1))
	{
		m_dead = true;
	}
}