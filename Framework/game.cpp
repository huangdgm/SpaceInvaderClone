// COMP710 GP 2D Framework 2017

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "playership.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, m_indexOfBullet(0)
, m_indexOfExplosion(0)
, m_indexOfAnimatedSprite(0)
{
	// Dong: Initialise all the elements in the enemy array to zero.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			m_enemy2DArray[row][col] = 0;
		}
	}

	// Dong: Initialise all the elements in the bullet array to zero.
	for (int index = 0; index < m_maxNumOfBullets; index++)
	{
		m_bulletArray[index] = 0;
	}

	// Dong: Initialise all the elements in the explosion array to zero.
	for (int index = 0; index < m_maxNumOfExplosions; index++)
	{
		m_explosionArray[index] = 0;
	}

	// Dong: Initialise all the elements in the animated sprit array to zero.
	for (int index = 0; index < m_maxNumOfAnimatedSprite; index++)
	{
		m_AnimatedSpriteArray[index] = 0;
	}
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_pPlayerShip;
	m_pPlayerShip = 0;

	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			delete m_enemy2DArray[row][col];
			m_enemy2DArray[row][col] = 0;
		}
	}

	for (int index = 0; index < m_maxNumOfBullets; index++)
	{
		delete m_bulletArray[index];
		m_bulletArray[index] = 0;
	}

	for (int index = 0; index < m_maxNumOfExplosions; index++)
	{
		delete m_explosionArray[index];
		m_explosionArray[index] = 0;
	}

	delete m_pEnemySprite;
	m_pEnemySprite = 0;

	delete m_pPlayerSprite;
	m_pPlayerSprite = 0;

	delete m_pPlayerBulletSprite;
	m_pPlayerBulletSprite = 0;

	for (int index = 0; index < m_maxNumOfAnimatedSprite; index++)
	{
		delete m_AnimatedSpriteArray[index];
		m_AnimatedSpriteArray[index] = 0;
	}
}

bool
Game::Initialise()
{
	// Dong: Create the back buffer
	m_pBackBuffer = new BackBuffer();

	if (!m_pBackBuffer->Initialise(Game::m_widthOfWindow, Game::m_heightOfWindow))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	// Dong: Create the input handler.
	m_pInputHandler = new InputHandler();

	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	// W03.1: Load the player ship sprite.
	// W03.1: Create the player ship instance.
	m_pPlayerShip = new PlayerShip();
	m_pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	m_pPlayerShip->Initialise(m_pPlayerSprite);	// initialise the playerShip with the sprite

	// W03.2: Spawn 4 rows, each row with 14 alien enemies.
	// W03.2: Fill the container with these new enemies.
	// dong: Set the sprite for the enemy. All the enemies share the same sprite.
	// dong: Set the sprite for the bullet. All the bullets share the same sprite.
	// dong: Because all the enemies share the same sprite, so all the enemies share the same texture instance. This is true for the bullet as well.
	m_pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	m_pPlayerBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");

	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			// dong: Create a new enemy and place it at the specified row and column.
			SpawnEnemy(row, col);
		}
	}

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}

		Draw(*m_pBackBuffer);
	}

	//	SDL_Delay(1);

	return (m_looping);
}

void
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:
	// Ex003.5: Process each alien enemy in the container.
	// Dong: Only process the enemy when this enemy is alive.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			if (m_enemy2DArray[row][col])
			{
				m_enemy2DArray[row][col]->Process(deltaTime);
			}
		}
	}

	// W03.3: Process each bullet in the container.
	// Dong: Only process the bullet when this bullet is alive.
	for (int index = 0; index < m_maxNumOfBullets; index++)
	{
		if (m_bulletArray[index])
		{
			m_bulletArray[index]->Process(deltaTime);
		}
	}

	// W03.1: Update player...
	m_pPlayerShip->Process(deltaTime);

	// W03.3: Check for bullet vs alien enemy collisions...
	// W03.3: For each bullet
	// W03.3: For each alien enemy
	// W03.3: Check collision between two entities.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			for (int index = 0; index < Game::m_maxNumOfBullets; index++)
			{
				if (m_bulletArray[index] != 0 && m_enemy2DArray[row][col] != 0)
				{
					if ((*m_bulletArray[index]).IsCollidingWith(*m_enemy2DArray[row][col]))
					{
						SpawnExplosion(row, col);

						// Set the value of dead enemy and dead bullet to zero.
						m_bulletArray[index] = 0;
						m_enemy2DArray[row][col] = 0;
					}
				}
			}
		}
	}

	for (int index = 0; index < Game::m_maxNumOfExplosions; index++)
	{
		if (m_explosionArray[index] != 0 && m_explosionArray[index]->IsExplosing())
		{
			m_explosionArray[index]->Process(deltaTime);
		}
	}

	// W03.3: Remove any dead explosions from the container...
}

void
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// W03.2: Draw all enemy aliens in container...
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			// The dead enemy has a value of 0.
			if (m_enemy2DArray[row][col])
			{
				m_enemy2DArray[row][col]->Draw(backBuffer);
			}
		}
	}

	// W03.3: Draw all bullets in container...
	for (int index = 0; index < Game::m_maxNumOfBullets; index++)
	{
		// The dead bullet has a value of 0.
		if (m_bulletArray[index])
		{
			m_bulletArray[index]->Draw(backBuffer);
		}
	}

	// W03.1: Draw the player ship...
	m_pPlayerShip->Draw(backBuffer);

	// Dong: Draw the explosion only if the explosion is ongoing.
	for (int index = 0; index < Game::m_maxNumOfExplosions; index++)
	{
		if (m_explosionArray[index] != 0 && m_explosionArray[index]->IsExplosing())
		{
			m_explosionArray[index]->Draw(backBuffer);
		}
	}

	backBuffer.Present();
}

void
Game::Quit()
{
	m_looping = false;
}

void
Game::StopSpaceShip()
{
	// Stop the spaceship once the dpad released.
	m_pPlayerShip->SetHorizontalVelocity(0);
}

void
Game::MoveSpaceShipLeft()
{
	// W03.1: Tell the player ship to move left.
	m_pPlayerShip->SetHorizontalVelocity(Game::m_velocityOfPlayerShip * (-1.0f));
}

// W03.1: Add the method to tell the player ship to move right...
void
Game::MoveSpaceShipRight()
{
	// Tell the player ship to move right.
	m_pPlayerShip->SetHorizontalVelocity(Game::m_velocityOfPlayerShip * 1.0f);
}

// W03.3: Space a Bullet in game.
void
Game::FireSpaceShipBullet()
{
	// W03.3: Load the player bullet sprite.

	// W03.3: Create a new bullet object.
	Bullet* bullet = new Bullet();

	// Initialise the initial position and the sprite of the bullet.
	bullet->Initialise(m_pPlayerBulletSprite);

	float positionX = (m_pPlayerShip->GetPositionX() + (m_pPlayerSprite->GetWidth() - m_pPlayerBulletSprite->GetWidth()) / 2) * 1.0f;
	float positionY = (Game::m_heightOfWindow - m_pPlayerSprite->GetHeight() - m_pPlayerBulletSprite->GetHeight()) * 1.0f;
	
	bullet->SetPositionX(positionX);
	bullet->SetPositionY(positionY);

	// W03.3: Set the bullets vertical velocity.
	bullet->SetVerticalVelocity(Game::m_velocityOfBullet * 1.0f);

	// W03.3: Add the new bullet to the bullet container.
	if (m_indexOfBullet < m_maxNumOfBullets)
	{
		m_bulletArray[m_indexOfBullet] = bullet;
		m_indexOfBullet++;
	}
	else
	{
		m_indexOfBullet = 0;
	}
}

// W03.2: Spawn a Enemy in game.
void
Game::SpawnEnemy(int row, int col)
{
	// W03.2: Create a new Enemy object.
	Enemy* enemy = new Enemy();

	// W03.2: Load the alien enemy sprite file.
	enemy->Initialise(m_pEnemySprite);
	enemy->SetPosition(59.0f*col, 59.0f*row);

	// W03.2: Add the new Enemy to the enemy container.
	m_enemy2DArray[row][col] = enemy;
}

void
Game::SpawnExplosion(int row, int col)
{
	Explosion* explosion = new Explosion();

	// Load an animated sprite for each explosion.
	AnimatedSprite* m_pAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");

	m_AnimatedSpriteArray[m_indexOfAnimatedSprite++] = m_pAnimatedSprite;

	explosion->Initialise(m_pAnimatedSprite);
	explosion->SetPositionX(59.0f*col);
	explosion->SetPositionY(59.0f*row);

	if (m_indexOfExplosion < m_maxNumOfExplosions)
	{
		m_explosionArray[m_indexOfExplosion++] = explosion;
	}
}