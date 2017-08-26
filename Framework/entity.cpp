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
, m_dead(false)
{

}

Entity::~Entity()
{

}

bool
Entity::Initialise(Sprite* sprite)	// Why Sprite*, not Sprite&
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);	// If the initilisation is successful, return true, otherwise, assert will stop the executing of the program.
}

void
Entity::Process(float deltaTime)
{
	// W03.1: Generic position update, based upon velocity (and time).
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;

	// W03.1: Boundary checking and position capping.
	// dong: General boundary check. Subclasses may need to modify the boundary accordingly.
	// dong: Here, only horizontal boundary check is performed.
	if (m_x <= 0)
	{
		m_x = 0;
	}
	else if (m_x >= 800)
	{
		m_x = 800;
	}
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

	// W03.3: Generic Entity Collision routine.

	// W03.3: Does this object collide with the e object?
	// W03.3: Create a circle for each entity (this and e).
	double radiusOfEnemy = sqrt(32 * 32 + 32 * 32) / 2;
	double radiusOfBullet = sqrt(16 * 16 + 16 * 16) / 2;
	
	distance = sqrt(pow((e.GetPositionX() - this->GetPositionX()), 2) + pow((e.GetPositionY() - this->GetPositionY()), 2));

	// W03.3: Check for intersection.
	// W03.3: Using circle-vs-circle collision detection.
	if (distance <= radiusOfBullet + radiusOfEnemy)
	{
		isCollid = true;
	}

	// W03.3: Return result of collision.
	return isCollid; // W03.4 Change return value!
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool
Entity::IsDead() const	// Add the implementation of IsDead()
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
