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
{

}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	const int numOfEnemyRows = 4;
	const int numOfEnemyCols = 14;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	// W03.1: Load the player ship sprite.
	// For example: Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");

	// W03.1: Create the player ship instance.
	m_pPlayerShip = new PlayerShip();
	m_pPlayerShip->Initialise(pPlayerSprite);	// initialise the playerShip with the sprite

	// W03.2: Spawn 4 rows, each row with 14 alien enemies.
	// W03.2: Fill the container with these new enemies.
	for (int row = 0; row < numOfEnemyRows; row++)
	{
		for (int col = 0; col < numOfEnemyCols; col++)
		{
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
	for (Enemy* enemy : enemyContainer)
	{
		enemy->Process(deltaTime);
	}

	// W03.3: Process each bullet in the container.
	for (Bullet* bullet : bulletContainer)
	{
		bullet->Process(deltaTime);
	}

	// W03.1: Update player...
	m_pPlayerShip->Process(deltaTime);

	// W03.3: Check for bullet vs alien enemy collisions...
	// W03.3: For each bullet
	// W03.3: For each alien enemy
	// W03.3: Check collision between two entities.

	for (vector<Bullet*>::iterator bulletIt = bulletContainer.begin(); bulletIt != bulletContainer.end();)
	{
		for (vector<Enemy*>::iterator enemyIt = enemyContainer.begin(); enemyIt != enemyContainer.end();)
		{
			if ((**bulletIt).IsCollidingWith(**enemyIt))
			{
				bulletContainer.erase(bulletIt);
				enemyContainer.erase(enemyIt);
			}
		}
	}

	//for (Bullet* bullet : bulletContainer)
	//{
	//	for (Enemy* enemy : enemyContainer)
	//	{
	//		if (bullet != 0 && enemy != 0)
	//		{
	//			if (bullet->IsCollidingWith(*enemy))
	//			{
	//				bullet->SetDead(true);
	//				enemy->SetDead(true);

	//				// W03.3: If collided, destory both and spawn explosion.
	//				// W03.3: Remove any dead bullets from the container...
	//				// W03.3: Remove any dead enemy aliens from the container...
	//				// enemyContainer.erase(std::remove(enemyContainer.begin(), enemyContainer.end(), bullet), enemyContainer.end());
	//				//std::vector<Enemy*>::iterator it = enemyContainer.begin();
	//				//enemyContainer.
	//				//enemyContainer.remove(enemy);
	//				//bulletContainer.remove(bullet);

	//				// enemyContainer.erase(distance(enemyContainer.begin(), find(enemyContainer.begin(), enemyContainer.end(), bullet)));
	//				//std::rotate(it, it + 1, enemyContainer.end());
	//				//enemyContainer.pop_back();
	//				// bulletContainer.erase(std::remove(bulletContainer.begin(), bulletContainer.end(), bullet), bulletContainer.end());
	//			}
	//		}
	//	}
	//}

	// W03.3: Remove any dead explosions from the container...
}

void
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// W03.2: Draw all enemy aliens in container...
	for (Enemy* enemy : enemyContainer)
	{
		enemy->Draw(backBuffer);

		/*if (!(enemy->IsDead()))
		{
		}*/
	}

	// W03.3: Draw all bullets in container...
	for (Bullet* bullet : bulletContainer)
	{
		bullet->Draw(backBuffer);

		/*if (!(bullet->IsDead()))
		{
		}*/
	}

	// W03.1: Draw the player ship...
	m_pPlayerShip->Draw(backBuffer);

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
	m_pPlayerShip->SetHorizontalVelocity(-50);
}

// W03.1: Add the method to tell the player ship to move right...
void
Game::MoveSpaceShipRight()
{
	// Tell the player ship to move right.
	m_pPlayerShip->SetHorizontalVelocity(50);
}

// W03.3: Space a Bullet in game.
void
Game::FireSpaceShipBullet()
{
	// W03.3: Load the player bullet sprite.
	Sprite* pPlayerBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");

	// W03.3: Create a new bullet object.
	Bullet* bullet = new Bullet();

	// Initialise the initial position and the sprite of the bullet.
	bullet->SetPositionY(552.0f);
	bullet->SetPositionX(m_pPlayerShip->GetPositionX() + 8.0f);
	bullet->Initialise(pPlayerBulletSprite);

	// W03.3: Set the bullets vertical velocity.
	bullet->SetVerticalVelocity(-100);

	// W03.3: Add the new bullet to the bullet container.
	bulletContainer.push_back(bullet);
}

// W03.2: Spawn a Enemy in game.
void
Game::SpawnEnemy(int row, int col)
{
	// W03.2: Create a new Enemy object.
	Enemy* enemy = new Enemy();

	// W03.2: Load the alien enemy sprite file.
	Sprite* pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");

	enemy->Initialise(pEnemySprite);
	enemy->SetPosition(59.0f*col, 59.0f*row);

	// W03.2: Add the new Enemy to the enemy container.
	enemyContainer.push_back(enemy);
}
