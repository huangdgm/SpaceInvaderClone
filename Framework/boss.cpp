// COMP710 GP 2D Framework 2017

// This include:
#include "boss.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"

// Library includes:

Boss::Boss()
: Enemy()
, m_health(1000)
, m_weaponType(TRIPLE_BULLET)
{

}

Boss::~Boss()
{

}

bool
Boss::Initialise(Sprite* sprite)
{
	Enemy::Initialise(sprite);

	m_dead = false;

	return true;
}

void
Boss::Process(float deltaTime)
{
	Enemy::Process(deltaTime);

	if (m_x <= 0)
	{
		m_x = 0;
	}
	else if (m_x >= Game::WIDTH_OF_PLAYING_PANEL - m_pSprite->GetWidth())
	{
		m_x = (Game::WIDTH_OF_PLAYING_PANEL - m_pSprite->GetWidth()) * 1.0f;
	}
}

void
Boss::SetHealth(int health)
{
	m_health = health;
}

int
Boss::GetHealth() const
{
	return m_health;
}
