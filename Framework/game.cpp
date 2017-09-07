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
#include <SDL_mixer.h>

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
, m_pEnemySprite(0)
, m_pPlayerSprite(0)
, m_pPlayerBulletSprite(0)
, m_pBackgroundSprite(0)
, m_scrollingOffset(0)
, m_pPlayerShip(0)
, m_pBackgroundMusic(0)
, m_pExplosionSoundEffect(0)
, m_pBulletSoundEffect(0)
{
	// Dong: Initialise all the elements in the enemy array to zero.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			m_enemy2DArray[row][col] = NULL;
		}
	}

	// Dong: Initialise all the elements in the bullet array to zero.
	for (int index = 0; index < m_maxNumOfBullets; index++)
	{
		m_bulletArray[index] = NULL;
	}

	// Dong: Initialise all the elements in the explosion array to zero.
	for (int index = 0; index < m_maxNumOfExplosions; index++)
	{
		m_explosionArray[index] = NULL;
	}

	// Dong: Initialise all the elements in the animated sprit array to zero.
	for (int index = 0; index < m_maxNumOfAnimatedSprite; index++)
	{
		m_AnimatedSpriteArray[index] = NULL;
	}
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = NULL;

	delete m_pInputHandler;
	m_pInputHandler = NULL;

	delete m_pPlayerShip;
	m_pPlayerShip = NULL;

	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			delete m_enemy2DArray[row][col];
			m_enemy2DArray[row][col] = NULL;
		}
	}

	for (int index = 0; index < m_maxNumOfBullets; index++)
	{
		delete m_bulletArray[index];
		m_bulletArray[index] = NULL;
	}

	for (int index = 0; index < m_maxNumOfExplosions; index++)
	{
		delete m_explosionArray[index];
		m_explosionArray[index] = NULL;
	}

	delete m_pEnemySprite;
	m_pEnemySprite = NULL;

	delete m_pPlayerSprite;
	m_pPlayerSprite = NULL;

	delete m_pPlayerBulletSprite;
	m_pPlayerBulletSprite = NULL;

	delete m_pBackgroundSprite;
	m_pBackgroundSprite = NULL;

	Mix_FreeMusic(m_pBackgroundMusic);
	m_pBackgroundMusic = NULL;

	Mix_FreeChunk(m_pExplosionSoundEffect);
	m_pExplosionSoundEffect = NULL;

	Mix_FreeChunk(m_pBulletSoundEffect);
	m_pBulletSoundEffect = NULL;
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

	// Load sprite.
	m_pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	m_pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	m_pPlayerBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
	m_pBackgroundSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\background.png");

	// Load audio.
	m_pBackgroundMusic = Mix_LoadMUS("assets/background.wav");;
	m_pExplosionSoundEffect = Mix_LoadWAV("assets/explosion.wav");
	m_pBulletSoundEffect = Mix_LoadWAV("assets/bullet.wav");

	m_pPlayerShip = new PlayerShip();
	m_pPlayerShip->Initialise(m_pPlayerSprite);

	// Spawn 4 rows, each row with 14 alien enemies.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			// Create a new enemy and place it at the specified row and column.
			SpawnEnemy(row, col);
		}
	}

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	// Play background music.
	Mix_PlayMusic(m_pBackgroundMusic, -1);

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

	// Process each alien enemy in the container.
	// Only process the enemy when this enemy is alive.
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

	// Process each bullet in the container.
	// Only process the bullet when this bullet is alive.
	for (int index = 0; index < m_maxNumOfBullets; index++)
	{
		if (m_bulletArray[index])
		{
			m_bulletArray[index]->Process(deltaTime);
		}
	}

	// Update player...
	if (m_pPlayerShip)
	{
		m_pPlayerShip->Process(deltaTime);
	}

	// Check for bullet vs alien enemy collisions...
	// Check collision between two entities.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			for (int index = 0; index < Game::m_maxNumOfBullets; index++)
			{
				if (m_bulletArray[index] != NULL && m_enemy2DArray[row][col] != NULL)
				{
					if ((*m_bulletArray[index]).IsCollidingWith(*m_enemy2DArray[row][col]))
					{
						float x = m_enemy2DArray[row][col]->GetPositionX();
						float y = m_enemy2DArray[row][col]->GetPositionY();

						SpawnExplosion(x, y);

						// Set the value of dead enemy and dead bullet to zero.
						m_bulletArray[index] = NULL;
						m_enemy2DArray[row][col] = NULL;
					}
				}
			}
		}
	}

	// Process all the explosions that are currently explosing.
	for (int index = 0; index < Game::m_maxNumOfExplosions; index++)
	{
		if (m_explosionArray[index] != NULL && m_explosionArray[index]->IsExplosing())
		{
			m_explosionArray[index]->Process(deltaTime);
		}
	}

	// Check for playership vs allien enemies collision.
	for (int row = 0; row < Game::m_numOfEnemyRows; row++)
	{
		for (int col = 0; col < Game::m_numOfEnemyCols; col++)
		{
			if (m_enemy2DArray[row][col] != NULL && m_pPlayerShip != NULL)
			{
				if ((*m_pPlayerShip).IsCollidingWith(*m_enemy2DArray[row][col]))
				{
					float x = m_enemy2DArray[row][col]->GetPositionX();
					float y = m_enemy2DArray[row][col]->GetPositionY();

					SpawnExplosion(x, y);

					// Set the value of dead enemy and dead bullet to zero.
					m_pPlayerShip = NULL;
					m_enemy2DArray[row][col] = NULL;
				}
			}
		}
	}

	// W03.3: Remove any dead explosions from the container...
}

void
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// Draw the scrolling background.
	// To make the scrolling slower, the m_scrollingOffset is divided by 60.
	m_scrollingOffset++;

	if ((m_scrollingOffset / 60) > m_pBackgroundSprite->GetHeight())
	{
		m_scrollingOffset = 0;
	}

	if (m_pBackgroundSprite)
	{
		m_pBackgroundSprite->DrawScrollingBackground(backBuffer, m_scrollingOffset / 60);
	}

	// Draw all enemy aliens in container...
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
	if (m_pPlayerShip)
	{
		m_pPlayerShip->Draw(backBuffer);
	}

	// Dong: Draw the explosion only if the explosion is ongoing.
	for (int index = 0; index < Game::m_maxNumOfExplosions; index++)
	{
		if (m_explosionArray[index] != NULL && m_explosionArray[index]->IsExplosing())
		{
			m_explosionArray[index]->Draw(backBuffer);
		}
	}

	// Update the screen.
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
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetHorizontalVelocity(0);
		m_pPlayerShip->SetVerticalVelocity(0);
	}
}

void
Game::MoveSpaceShipLeft()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetHorizontalVelocity(Game::m_velocityOfPlayerShip * (-1.0f));
	}
}

void
Game::MoveSpaceShipRight()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetHorizontalVelocity(Game::m_velocityOfPlayerShip * 1.0f);
	}
}

void
Game::MoveSpaceShipUp()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetVerticalVelocity(Game::m_velocityOfPlayerShip * (-1.0f));
	}
}

void
Game::MoveSpaceShipDown()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetVerticalVelocity(Game::m_velocityOfPlayerShip * (1.0f));
	}
}

void
Game::FireSpaceShipBullet()
{
	Bullet* bullet = new Bullet();

	bullet->Initialise(m_pPlayerBulletSprite);

	assert(m_pPlayerShip);

	float positionX = (m_pPlayerShip->GetPositionX() + (m_pPlayerSprite->GetWidth() - m_pPlayerBulletSprite->GetWidth()) / 2) * 1.0f;
	float positionY = (m_pPlayerShip->GetPositionY() - m_pPlayerBulletSprite->GetHeight()) * 1.0f;

	bullet->SetPositionX(positionX);
	bullet->SetPositionY(positionY);

	bullet->SetVerticalVelocity(Game::m_velocityOfBullet * 1.0f);

	// Add the new bullet to the bullet container.
	if (m_indexOfBullet < m_maxNumOfBullets)
	{
		m_bulletArray[m_indexOfBullet] = bullet;
		m_indexOfBullet++;
	}
	else
	{
		m_indexOfBullet = 0;
	}

	// Play the bullet sound effect.
	Mix_PlayChannel(-1, m_pBulletSoundEffect, 0);
}

void
Game::SpawnEnemy(int row, int col)
{
	Enemy* enemy = new Enemy();

	enemy->Initialise(m_pEnemySprite);
	enemy->SetPosition(59.0f*col, 59.0f*row);

	// To make the enemies to move towards the bottom of the screen.
	enemy->SetVerticalVelocity(10);

	m_enemy2DArray[row][col] = enemy;
}

void
Game::SpawnExplosion(float x, float y)
{
	Explosion* explosion = new Explosion();

	// Load an animated sprite for each explosion.
	AnimatedSprite* m_pAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");

	m_AnimatedSpriteArray[m_indexOfAnimatedSprite++] = m_pAnimatedSprite;

	explosion->Initialise(m_pAnimatedSprite);
	explosion->SetPositionX(x);
	explosion->SetPositionY(y);

	if (m_indexOfExplosion < m_maxNumOfExplosions)
	{
		m_explosionArray[m_indexOfExplosion++] = explosion;
	}

	// Play the explosion sound effect.
	Mix_PlayChannel(-1, m_pExplosionSoundEffect, 0);
}