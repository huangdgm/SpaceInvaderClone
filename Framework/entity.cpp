// COMP710 GP 2D Framework 2017

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <cmath>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(true)
{

}

Entity::~Entity()
{

}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);

	m_pSprite = sprite;

	return (true);
}

void
Entity::Process(float deltaTime)
{
	// Generic position update, based upon velocity (and time).
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);

	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));

	m_pSprite->Draw(backBuffer);
}

bool
Entity::IsCollidingWith(Entity& e)
{
	double distance = 0.0;
	bool isCollid = false;

	double this_radius = this->m_pSprite->GetHeight() / sqrt(2);
	double entity_radius = e.m_pSprite->GetHeight() / sqrt(2);
	
	float this_CenterX = this->GetPositionX() + (m_pSprite->GetWidth() / 2);
	float this_CenterY = this->GetPositionY() + (m_pSprite->GetHeight() / 2);
	float entity_CenterX = e.GetPositionX() + (e.m_pSprite->GetWidth() / 2);
	float entity_CenterY = e.GetPositionY() + (e.m_pSprite->GetHeight() / 2);

	distance = sqrt(pow((entity_CenterX - this_CenterX), 2) + pow((entity_CenterY - this_CenterY), 2));

	// Check for intersection.
	if (distance <= this_radius + entity_radius)
	{
		isCollid = true;
	}

	return isCollid;
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool
Entity::IsDead() const
{
	return m_dead;
}

void
Entity::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetPositionX() const
{
	return (m_x);
}

float 
Entity::GetPositionY() const
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity() const
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity() const
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}
