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
#include "infopanel.h"
#include "texttexture.h"

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
, m_indexOfEnemy(0)
, m_indexOfExplosion(0)
, m_pEnemySprite(0)
, m_pPlayerSprite(0)
, m_pPlayerBulletSprite(0)
, m_pBackgroundSprite(0)
, m_pInfoPanelSprite(0)
, m_scrollingOffset(0)
, m_pPlayerShip(0)
, m_pBackgroundMusic(0)
, m_pExplosionSoundEffect(0)
, m_pBulletSoundEffect(0)
, m_pEnemy(new Enemy[NUM_OF_ENEMY_COLS])
, m_pBullet(new Bullet[MAX_NUM_OF_BULLETS])
, m_pExplosion(new Explosion[MAX_NUM_OF_EXPLOSIONS])
, m_pFont(0)
, m_pTextTexture(0)
, m_pFontColor(0)
{

}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = NULL;

	delete m_pInputHandler;
	m_pInputHandler = NULL;

	delete m_pPlayerShip;
	m_pPlayerShip = NULL;

	delete m_pInfoPanel;
	m_pInfoPanel = NULL;

	delete m_pEnemySprite;
	m_pEnemySprite = NULL;

	delete m_pPlayerSprite;
	m_pPlayerSprite = NULL;

	delete m_pPlayerBulletSprite;
	m_pPlayerBulletSprite = NULL;

	delete m_pBackgroundSprite;
	m_pBackgroundSprite = NULL;

	delete m_pInfoPanelSprite;
	m_pInfoPanelSprite = NULL;

	delete[] m_pEnemy;
	m_pEnemy = NULL;

	delete[] m_pBullet;
	m_pBullet = NULL;

	delete[] m_pExplosion;
	m_pExplosion = NULL;

	Mix_FreeMusic(m_pBackgroundMusic);
	m_pBackgroundMusic = NULL;

	Mix_FreeChunk(m_pExplosionSoundEffect);
	m_pExplosionSoundEffect = NULL;

	Mix_FreeChunk(m_pBulletSoundEffect);
	m_pBulletSoundEffect = NULL;

	TTF_CloseFont(m_pFont);
	m_pFont = NULL;

	delete m_pTextTexture;
	m_pTextTexture = NULL;

	delete m_pFontColor;
	m_pFontColor = NULL;
}

bool
Game::Initialise()
{
	// Create the back buffer.
	CreateBackBuffer();

	// Create the input handler.
	CreateInputHandler();

	// Create the TTF font.
	CreateTTFFont();

	// Create the font color.
	CreateFontColor(0, 0, 0);

	// Load sprite.
	m_pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	m_pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	m_pPlayerBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
	m_pBackgroundSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\background.png");
	m_pInfoPanelSprite = m_pBackBuffer->CreateSprite("assets\\infopanel.png");

	// Load audio.
	m_pBackgroundMusic = Mix_LoadMUS("assets\\background.wav");;
	m_pExplosionSoundEffect = Mix_LoadWAV("assets\\explosion.wav");
	m_pBulletSoundEffect = Mix_LoadWAV("assets\\bullet.wav");

	// Create the player ship.
	m_pPlayerShip = new PlayerShip();
	m_pPlayerShip->Initialise(m_pPlayerSprite);

	// Create the enemies.
	for (int i = 0; i < NUM_OF_ENEMY_COLS; i++)
	{
		SpawnEnemy(0, i);
	}

	// Create the info panel.
	m_pInfoPanel = new InfoPanel();
	m_pInfoPanel->Initialise(m_pInfoPanelSprite);

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	// Play background music.
	Mix_PlayMusic(m_pBackgroundMusic, -1);

	// Create the text texture.
	m_pTextTexture = new TextTexture();
	m_pTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());

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

	// Process the enemies.
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + NUM_OF_ENEMY_COLS; enemy++)
	{
		if (!(enemy->IsDead()))
		{
			enemy->Process(deltaTime);
		}
	}

	// Process the bullets.
	for (Bullet* bullet = m_pBullet; bullet < m_pBullet + MAX_NUM_OF_BULLETS; bullet++)
	{
		if (!(bullet->IsDead()))
		{
			bullet->Process(deltaTime);
		}
	}

	// Process the player.
	if (m_pPlayerShip)
	{
		m_pPlayerShip->Process(deltaTime);
	}

	// Check for the collisions between the enemies and the bullets.
	// If collisions detected, spawn the explosions.
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + NUM_OF_ENEMY_COLS; enemy++)
	{
		for (Bullet* bullet = m_pBullet; bullet < m_pBullet + MAX_NUM_OF_BULLETS; bullet++)
		{
			if (!(enemy->IsDead()) && !(bullet->IsDead()))
			{
				if (bullet->IsCollidingWith(*enemy))
				{
					float x = enemy->GetPositionX();
					float y = enemy->GetPositionY();

					SpawnExplosion(x, y);
					m_pPlayerShip->SetScore(m_pPlayerShip->GetScore() + 10);

					enemy->SetDead(true);
					bullet->SetDead(true);
				}
			}
		}
	}

	// Process the explosions.
	for (Explosion* explosion = m_pExplosion; explosion < m_pExplosion + MAX_NUM_OF_EXPLOSIONS; explosion++)
	{
		if (!(explosion->IsDead()) && explosion->IsExplosing())
		{
			explosion->Process(deltaTime);
		}
	}

	// Check for the collisions between the enemies and the player ship.
	// If collision detected, spawn the explosion.
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + NUM_OF_ENEMY_COLS; enemy++)
	{
		if (!(enemy->IsDead()) && !(m_pPlayerShip->IsDead()))
		{
			if (m_pPlayerShip->IsCollidingWith(*enemy))
			{
				float x = enemy->GetPositionX();
				float y = enemy->GetPositionY();

				SpawnExplosion(x, y);

				enemy->SetDead(true);
				m_pPlayerShip->SetDead(true);
			}
		}
	}

	// Process the text texture.
	m_pTextTexture->LoadFromRenderedText(to_string(m_pPlayerShip->GetScore()), *m_pFontColor);

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

	// Draw the info panel
	if (!(m_pInfoPanel->IsDead()))
	{
		m_pInfoPanel->Draw(backBuffer);
	}

	// Draw the enemies.
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + NUM_OF_ENEMY_COLS; enemy++)
	{
		if (!(enemy->IsDead()))
		{
			enemy->Draw(backBuffer);
		}
	}

	// Draw the bullets.
	for (Bullet* bullet = m_pBullet; bullet < m_pBullet + MAX_NUM_OF_BULLETS; bullet++)
	{
		if (!(bullet->IsDead()))
		{
			bullet->Draw(backBuffer);
		}
	}

	// Draw the player ship.
	if (!(m_pPlayerShip->IsDead()))
	{
		m_pPlayerShip->Draw(backBuffer);
	}

	// Draw the explosions.
	for (Explosion* explosion = m_pExplosion; explosion < m_pExplosion + MAX_NUM_OF_EXPLOSIONS; explosion++)
	{
		if (!(explosion->IsDead()) && explosion->IsExplosing())
		{
			explosion->Draw(backBuffer);
		}
	}

	// Draw the text texture.
	m_pTextTexture->Render(620, 100);

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
		m_pPlayerShip->SetHorizontalVelocity(VELOCITY_OF_PLAYERSHIP * (-1.0f));
	}
}

void
Game::MoveSpaceShipRight()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetHorizontalVelocity(VELOCITY_OF_PLAYERSHIP * 1.0f);
	}
}

void
Game::MoveSpaceShipUp()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetVerticalVelocity(VELOCITY_OF_PLAYERSHIP * (-1.0f));
	}
}

void
Game::MoveSpaceShipDown()
{
	if (m_pPlayerShip)
	{
		m_pPlayerShip->SetVerticalVelocity(VELOCITY_OF_PLAYERSHIP * (1.0f));
	}
}

void
Game::FireSpaceShipBullet()
{
	Bullet* bullet = m_pBullet;

	if (m_indexOfBullet < MAX_NUM_OF_BULLETS)
	{
		bullet = m_pBullet + m_indexOfBullet;

		m_indexOfBullet++;
	}
	else
	{
		// To limit the the number of bullets.
		m_indexOfBullet = 0;
	}

	bullet->Initialise(m_pPlayerBulletSprite);

	assert(m_pPlayerShip);

	float positionX = (m_pPlayerShip->GetPositionX() + (m_pPlayerSprite->GetWidth() - m_pPlayerBulletSprite->GetWidth()) / 2) * 1.0f;
	float positionY = (m_pPlayerShip->GetPositionY() - m_pPlayerBulletSprite->GetHeight()) * 1.0f;

	bullet->SetPositionX(positionX);
	bullet->SetPositionY(positionY);

	bullet->SetVerticalVelocity(VELOCITY_OF_BULLET * 1.0f);

	// Play the bullet sound effect.
	Mix_PlayChannel(-1, m_pBulletSoundEffect, 0);
}

void
Game::SpawnEnemy(int row, int col)
{
	Enemy* enemy = NULL;

	if (m_indexOfEnemy < NUM_OF_ENEMY_COLS)
	{
		enemy = m_pEnemy + (m_indexOfEnemy++);
	}

	enemy->Initialise(m_pEnemySprite);
	enemy->SetPosition(59.0f*col, 59.0f*row);

	// To make the enemies to move towards the bottom of the screen.
	enemy->SetVerticalVelocity(50);
}

void
Game::SpawnExplosion(float x, float y)
{
	Explosion* explosion = NULL;

	if (m_indexOfExplosion < MAX_NUM_OF_EXPLOSIONS)
	{
		explosion = m_pExplosion + (m_indexOfExplosion++);
	}

	explosion->Initialise(m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png"));

	explosion->SetPositionX(x);
	explosion->SetPositionY(y);

	// Play the explosion sound effect.
	Mix_PlayChannel(-1, m_pExplosionSoundEffect, 0);
}

bool
Game::CreateBackBuffer()
{
	m_pBackBuffer = new BackBuffer();

	if (!m_pBackBuffer->Initialise(WIDTH_OF_WINDOW, HEIGHT_OF_WINDOW))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	return (true);
}

bool
Game::CreateInputHandler()
{
	m_pInputHandler = new InputHandler();

	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	return (true);
}

bool
Game::CreateTTFFont()
{
	m_pFont = TTF_OpenFont("assets\\stencil.ttf", 28);

	if (m_pFont == NULL)
	{
		LogManager::GetInstance().Log("Failed to load font!");
		return (false);
	}

	return (true);
}

bool
Game::CreateFontColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_pFontColor = new SDL_Color();

	if (m_pFontColor == NULL)
	{
		LogManager::GetInstance().Log("Failed to create font color!");
		return (false);
	}
	else
	{
		m_pFontColor->r = r;
		m_pFontColor->g = g;
		m_pFontColor->b = b;
	}

	return (true);
}