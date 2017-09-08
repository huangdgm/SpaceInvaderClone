// COMP710 GP 2D Framework 2017

// This include:
#include "explosion.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "animatedsprite.h"

// Library includes:
#include <cassert>
#include <Windows.h>

// Ensure the constructor chains-up to the super class constructor.
Explosion::Explosion()
: m_pAnimatedSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_dead(true)
{

}

Explosion::~Explosion()
{
	delete m_pAnimatedSprite;
	m_pAnimatedSprite = NULL;
}

bool
Explosion::Initialise(AnimatedSprite* animatedSprite)
{
	assert(animatedSprite);
	m_pAnimatedSprite = animatedSprite;

	m_pAnimatedSprite->AddFrame(0);
	m_pAnimatedSprite->AddFrame(64);
	m_pAnimatedSprite->AddFrame(128);
	m_pAnimatedSprite->AddFrame(192);
	m_pAnimatedSprite->AddFrame(256);

	m_pAnimatedSprite->ResetCurrentFrame();
	m_dead = false;

	return true;
}

void
Explosion::Process(float deltaTime)
{
	m_pAnimatedSprite->Process(deltaTime);
}

void
Explosion::Draw(BackBuffer& backbuffer)
{
	assert(m_pAnimatedSprite);

	// Set the position of the animated sprite.
	m_pAnimatedSprite->SetX(static_cast<int>(m_x));
	m_pAnimatedSprite->SetY(static_cast<int>(m_y));

	m_pAnimatedSprite->Draw(backbuffer);
}

void
Explosion::SetPositionX(float x)
{
	m_x = x;
}

void
Explosion::SetPositionY(float y)
{
	m_y = y;
}

float
Explosion::GetPositionX() const
{
	return (m_x);
}

float
Explosion::GetPositionY() const
{
	return (m_y);
}

bool
Explosion::IsExplosing() const
{
	return (m_pAnimatedSprite->IsAnimating());
}

bool
Explosion::IsDead() const
{
	return m_dead;
}

void
Explosion::SetDead(bool dead)
{
	m_dead = dead;
}