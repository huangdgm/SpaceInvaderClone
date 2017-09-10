// COMP710 GP 2D Framework 2017

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "playership.h"
#include "playerbullet.h"
#include "enemybullet.h"
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
, m_indexOfPlayerBullet(0)
, m_indexOfEnemyBullet(0)
, m_indexOfEnemy(0)
, m_indexOfPlayerShip(0)
, m_indexOfExplosion(0)
, m_pEnemySprite(0)
, m_pPlayerSprite(0)
, m_pPlayerBulletSprite(0)
, m_pEnemyBulletSprite(0)
, m_pBackgroundSprite(0)
, m_pInfoPanelSprite(0)
, m_scrollingOffset(0)
, m_pBackgroundMusic(0)
, m_pExplosionSoundEffect(0)
, m_pBulletSoundEffect(0)
, m_pFont(0)
, m_pFontColor(0)
, m_pScoreTextTexture(0)
, m_pLevelTextTexture(0)
, m_pLivesTextTexture(0)
, m_pHealthTextTexture(0)
, m_level(0)
, m_score(0)
{
	m_pEnemy = new Enemy[MAX_NUM_OF_ENEMY];
	m_pPlayerShip = new PlayerShip[MAX_NUM_OF_PLAYER_SHIP];
	m_pPlayerBullet = new PlayerBullet[MAX_NUM_OF_PLAYER_BULLETS];
	m_pEnemyBullet = new EnemyBullet[MAX_NUM_OF_ENEMY_BULLETS];
	m_pExplosion = new Explosion[MAX_NUM_OF_EXPLOSIONS];
	m_numOfLivesLeft = MAX_NUM_OF_PLAYER_SHIP - 1;
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = NULL;

	delete m_pInputHandler;
	m_pInputHandler = NULL;

	delete m_pInfoPanel;
	m_pInfoPanel = NULL;

	delete m_pEnemySprite;
	m_pEnemySprite = NULL;

	delete m_pPlayerSprite;
	m_pPlayerSprite = NULL;

	delete m_pPlayerBulletSprite;
	m_pPlayerBulletSprite = NULL;

	delete m_pEnemyBulletSprite;
	m_pEnemyBulletSprite = NULL;

	delete m_pBackgroundSprite;
	m_pBackgroundSprite = NULL;

	delete m_pInfoPanelSprite;
	m_pInfoPanelSprite = NULL;

	delete[] m_pEnemy;
	m_pEnemy = NULL;

	delete[] m_pPlayerShip;
	m_pPlayerShip = NULL;

	delete[] m_pPlayerBullet;
	m_pPlayerBullet = NULL;

	delete[] m_pEnemyBullet;
	m_pEnemyBullet = NULL;

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

	delete m_pFontColor;
	m_pFontColor = NULL;

	delete m_pScoreTextTexture;
	m_pScoreTextTexture = NULL;

	delete m_pLevelTextTexture;
	m_pLevelTextTexture = NULL;

	delete m_pLivesTextTexture;
	m_pLivesTextTexture = NULL;

	delete m_pHealthTextTexture;
	m_pHealthTextTexture = NULL;
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
	CreateFontColor(255, 255, 255);

	// Load sprite.
	m_pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	m_pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	m_pPlayerBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
	m_pEnemyBulletSprite = m_pBackBuffer->CreateSprite("assets\\enemybullet.png");
	m_pBackgroundSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\background.png");
	m_pInfoPanelSprite = m_pBackBuffer->CreateSprite("assets\\infopanel.png");

	// Load audio.
	m_pBackgroundMusic = Mix_LoadMUS("assets\\background.wav");;
	m_pExplosionSoundEffect = Mix_LoadWAV("assets\\explosion.wav");
	m_pBulletSoundEffect = Mix_LoadWAV("assets\\bullet.wav");

	// Create the player ship.
	SpawnPlayerShip();

	// Create the enemies.
	for (int i = 0; i < MAX_NUM_OF_ENEMY; i++)
	{
		SpawnEnemy(i*50, 0);
	}

	// Create the info panel.
	m_pInfoPanel = new InfoPanel();
	m_pInfoPanel->Initialise(m_pInfoPanelSprite);

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	// Play background music.
	Mix_PlayMusic(m_pBackgroundMusic, -1);

	// Create the score text texture.
	m_pScoreTextTexture = new TextTexture();
	m_pScoreTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());

	// Create the level text texture.
	m_pLevelTextTexture = new TextTexture();
	m_pLevelTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());

	// Create the lives text texture.
	m_pLivesTextTexture = new TextTexture();
	m_pLivesTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());

	// Create the health text texture.
	m_pHealthTextTexture = new TextTexture();
	m_pHealthTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());

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
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + MAX_NUM_OF_ENEMY; enemy++)
	{
		if (!(enemy->IsDead()))
		{
			enemy->Process(deltaTime);
		}
	}

	// Process the player bullets.
	for (PlayerBullet* playerBullet = m_pPlayerBullet; playerBullet < m_pPlayerBullet + MAX_NUM_OF_PLAYER_BULLETS; playerBullet++)
	{
		if (!(playerBullet->IsDead()))
		{
			playerBullet->Process(deltaTime);
		}
	}

	// Process the enemy bullets.
	for (EnemyBullet* enemyBullet = m_pEnemyBullet; enemyBullet < m_pEnemyBullet + MAX_NUM_OF_ENEMY_BULLETS; enemyBullet++)
	{
		if (!(enemyBullet->IsDead()))
		{
			enemyBullet->Process(deltaTime);
		}
	}

	// Process the player.
	for (PlayerShip* playerShip = m_pPlayerShip; playerShip < m_pPlayerShip + MAX_NUM_OF_PLAYER_SHIP; playerShip++)
	{
		if (!(playerShip->IsDead()))
		{
			playerShip->Process(deltaTime);
		}
	}

	// Check for the collisions between the enemies and the player bullets.
	// If collisions detected, spawn the explosions.
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + MAX_NUM_OF_ENEMY; enemy++)
	{
		for (PlayerBullet* bullet = m_pPlayerBullet; bullet < m_pPlayerBullet + MAX_NUM_OF_PLAYER_BULLETS; bullet++)
		{
			if (!(enemy->IsDead()) && !(bullet->IsDead()))
			{
				if (bullet->IsCollidingWith(*enemy))
				{
					float x = enemy->GetPositionX();
					float y = enemy->GetPositionY();

					// todo: change x, y
					SpawnExplosion(x, y);
					m_score += 10;

					enemy->SetDead(true);
					bullet->SetDead(true);
				}
			}
		}
	}

	if (rand() % 100 > 95)
	{
		SpawnEnemyBullet();
	}

	// Check for the collisions between the player ship and the enemy bullets.
	// If collisions detected, spawn the explosions.
	for (EnemyBullet* enemyBullet = m_pEnemyBullet; enemyBullet < m_pEnemyBullet + MAX_NUM_OF_ENEMY_BULLETS; enemyBullet++)
	{
		for (PlayerShip* playerShip = m_pPlayerShip; playerShip < m_pPlayerShip + MAX_NUM_OF_PLAYER_SHIP; playerShip++)
		{
			if (!(enemyBullet->IsDead()) && !(playerShip->IsDead()))
			{
				if (playerShip->IsCollidingWith(*enemyBullet))
				{
					float x = playerShip->GetPositionX();
					float y = playerShip->GetPositionY();

					UpdatePlayerShip(playerShip);
					enemyBullet->SetDead(true);
					SpawnExplosion(x, y);
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
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + MAX_NUM_OF_ENEMY; enemy++)
	{
		for (PlayerShip* playerShip = m_pPlayerShip; playerShip < m_pPlayerShip + MAX_NUM_OF_PLAYER_SHIP; playerShip++)
		{
			if (!(enemy->IsDead()) && !(playerShip->IsDead()))
			{
				if (playerShip->IsCollidingWith(*enemy))
				{
					float x = playerShip->GetPositionX();
					float y = playerShip->GetPositionY();

					UpdatePlayerShip(playerShip);
					enemy->SetDead(true);
					SpawnExplosion(x, y);
				}
			}
		}
	}

	// Process the text texture.
	m_pScoreTextTexture->LoadFromRenderedText(to_string(m_score), *m_pFontColor);
	m_pLevelTextTexture->LoadFromRenderedText(to_string(m_level), *m_pFontColor);
	m_pLivesTextTexture->LoadFromRenderedText(to_string(m_numOfLivesLeft), *m_pFontColor);
	// Todo: create the GetCurrentPlayerShip() method.
	m_pHealthTextTexture->LoadFromRenderedText(to_string((m_pPlayerShip + m_indexOfPlayerShip - 1)->GetHealth()), *m_pFontColor);

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
	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + MAX_NUM_OF_ENEMY; enemy++)
	{
		if (!(enemy->IsDead()))
		{
			enemy->Draw(backBuffer);
		}
	}

	// Draw the player bullets.
	for (PlayerBullet* playerBullet = m_pPlayerBullet; playerBullet < m_pPlayerBullet + MAX_NUM_OF_PLAYER_BULLETS; playerBullet++)
	{
		if (!(playerBullet->IsDead()))
		{
			playerBullet->Draw(backBuffer);
		}
	}

	// Draw the enemy bullets.
	for (EnemyBullet* enemyBullet = m_pEnemyBullet; enemyBullet < m_pEnemyBullet + MAX_NUM_OF_ENEMY_BULLETS; enemyBullet++)
	{
		if (!(enemyBullet->IsDead()))
		{
			enemyBullet->Draw(backBuffer);
		}
	}

	// Draw the player ship.
	for (PlayerShip* playerShip = m_pPlayerShip; playerShip < m_pPlayerShip + MAX_NUM_OF_PLAYER_SHIP; playerShip++)
	{
		if (!(playerShip->IsDead()))
		{
			playerShip->Draw(backBuffer);
		}
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
	m_pScoreTextTexture->Render(690, 60);
	m_pLevelTextTexture->Render(700, 250);
	m_pLivesTextTexture->Render(736, 410);
	m_pHealthTextTexture->Render(665, 580);

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
	PlayerShip* playerShip = m_pPlayerShip + m_indexOfPlayerShip - 1;

	playerShip->SetHorizontalVelocity(0);
	playerShip->SetVerticalVelocity(0);
}

void
Game::MoveSpaceShipLeft()
{
	PlayerShip* playerShip = m_pPlayerShip + m_indexOfPlayerShip - 1;

	playerShip->SetHorizontalVelocity(VELOCITY_OF_PLAYERSHIP * (-1.0f));
}

void
Game::MoveSpaceShipRight()
{
	PlayerShip* playerShip = m_pPlayerShip + m_indexOfPlayerShip - 1;

	playerShip->SetHorizontalVelocity(VELOCITY_OF_PLAYERSHIP * 1.0f);
}

void
Game::MoveSpaceShipUp()
{
	PlayerShip* playerShip = m_pPlayerShip + m_indexOfPlayerShip - 1;

	playerShip->SetVerticalVelocity(VELOCITY_OF_PLAYERSHIP * (-1.0f));
}

void
Game::MoveSpaceShipDown()
{
	PlayerShip* playerShip = m_pPlayerShip + m_indexOfPlayerShip - 1;

	playerShip->SetVerticalVelocity(VELOCITY_OF_PLAYERSHIP * (1.0f));
}

void
Game::FireSpaceShipBullet()
{
	PlayerBullet* bullet = m_pPlayerBullet;

	if (m_indexOfPlayerBullet < MAX_NUM_OF_PLAYER_BULLETS)
	{
		bullet = m_pPlayerBullet + m_indexOfPlayerBullet;

		m_indexOfPlayerBullet++;
	}
	else
	{
		// To limit the the number of bullets.
		m_indexOfPlayerBullet = 0;
	}

	bullet->Initialise(m_pPlayerBulletSprite);

	float positionX = ((m_pPlayerShip + m_indexOfPlayerShip - 1)->GetPositionX() + (m_pPlayerSprite->GetWidth() - m_pPlayerBulletSprite->GetWidth()) / 2) * 1.0f;
	float positionY = ((m_pPlayerShip + m_indexOfPlayerShip - 1)->GetPositionY() - m_pPlayerBulletSprite->GetHeight()) * 1.0f;

	bullet->SetPositionX(positionX);
	bullet->SetPositionY(positionY);

	bullet->SetVerticalVelocity(VELOCITY_OF_PLAYER_BULLET * 1.0f);

	// Play the bullet sound effect.
	Mix_PlayChannel(-1, m_pBulletSoundEffect, 0);
}

void
Game::SpawnEnemy(int x, int y)
{
	Enemy* enemy = NULL;

	if (m_indexOfEnemy < MAX_NUM_OF_ENEMY)
	{
		enemy = m_pEnemy + (m_indexOfEnemy++);
	}

	enemy->Initialise(m_pEnemySprite);
	enemy->SetPosition(x, y);

	// To make the enemies to move towards the bottom of the screen.
	enemy->SetVerticalVelocity(VELOCITY_OF_ENEMY * 1.0f);
}

void
Game::SpawnPlayerShip()
{
	PlayerShip* playerShip = NULL;

	if (m_indexOfPlayerShip < MAX_NUM_OF_PLAYER_SHIP)
	{
		playerShip = m_pPlayerShip + (m_indexOfPlayerShip++);
	}

	playerShip->Initialise(m_pPlayerSprite);

	float positionX = (Game::WIDTH_OF_PLAYING_PANEL - m_pPlayerBulletSprite->GetWidth()) / 2.0f;
	float positionY = (Game::HEIGHT_OF_PLAYING_PANEL - m_pPlayerBulletSprite->GetHeight()) * 1.0f;

	playerShip->SetPosition(positionX, positionY);
}

void
Game::SpawnEnemyBullet()
{
	EnemyBullet* enemyBullet = m_pEnemyBullet;

	if (m_indexOfEnemyBullet < MAX_NUM_OF_ENEMY_BULLETS)
	{
		enemyBullet = m_pEnemyBullet + m_indexOfEnemyBullet;

		m_indexOfEnemyBullet++;
	}
	else
	{
		// To limit the the number of bullets.
		m_indexOfEnemyBullet = 0;
	}

	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + MAX_NUM_OF_ENEMY; enemy++)
	{
		if (rand() % 100 > 96 && !(enemy->IsDead()))
		{
			enemyBullet->Initialise(m_pEnemyBulletSprite);

			float positionX = (enemy->GetPositionX() + m_pEnemySprite->GetWidth() / 2) * 1.0f;
			float positionY = (enemy->GetPositionY() + m_pEnemySprite->GetHeight()) * 1.0f;

			enemyBullet->SetPositionX(positionX);
			enemyBullet->SetPositionY(positionY);

			break;
		}
	}

	enemyBullet->SetVerticalVelocity(VELOCITY_OF_ENEMY_BULLET * 1.0f);
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

bool
Game::HasMoreLives()
{
	return (m_numOfLivesLeft >= 1);
}

void
Game::UpdatePlayerShip(PlayerShip* playerShip)
{
	if (playerShip->GetHealth() - DAMAGE_CAUSED_BY_ENEMY_BULLET <= 0)
	{
		if (HasMoreLives())
		{
			--m_numOfLivesLeft;
			SpawnPlayerShip();
		}
		else
		{
			// game over
		}

		playerShip->SetDead(true);
	}
	else
	{
		playerShip->SetHealth(playerShip->GetHealth() - DAMAGE_CAUSED_BY_ENEMY_BULLET);
	}
}