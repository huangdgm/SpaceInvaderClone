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
#include "gamestate.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <SDL_mixer.h>
#include <cmath>

using namespace std;

// Static Members:
Game* Game::sm_pInstance = 0;
GameState Game::sm_gameState = SPLASH_SCREEN;

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
, m_gamePlayLooping(true)
, m_splashScreenLooping(true)
, m_mainMenuLooping(true)
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
, m_indexOfExplosionAnimatedSprite(0)
, m_pEnemySprite(0)
, m_pPlayerSprite(0)
, m_pPlayerBulletSprite(0)
, m_pEnemyBulletSprite(0)
, m_pBackgroundSprite(0)
, m_pInfoPanelSprite(0)
, m_pPlayGameInMainMenuSprite(0)
, m_pQuitGameInMainMenuSprite(0)
, m_pSplashScreenSprite(0)
, m_scrollingOffset(0)
, m_pBackgroundMusic(0)
, m_pExplosionSoundEffect(0)
, m_pBulletSoundEffect(0)
, m_pHurtSoundEffect(0)
, m_pFont(0)
, m_pFontColor(0)
, m_pScoreTextTexture(0)
, m_pLevelTextTexture(0)
, m_pLivesTextTexture(0)
, m_pHealthTextTexture(0)
, m_level(1)
, m_score(0)
, m_playGameMenuInMainMenuSelected(true)
, m_quitGameMenuInMainMenuSelected(false)
, m_pSplashScreen(0)
, m_pMainMenu(0)
{
	m_numOfLivesLeft = MAX_NUM_OF_PLAYER_SHIP - 1;
	
	m_pEnemy = new Enemy[MAX_NUM_OF_ENEMY];
	m_pPlayerShip = new PlayerShip[MAX_NUM_OF_PLAYER_SHIP];
	m_pPlayerBullet = new PlayerBullet[MAX_NUM_OF_PLAYER_BULLETS];
	m_pEnemyBullet = new EnemyBullet[MAX_NUM_OF_ENEMY_BULLETS];
	m_pExplosion = new Explosion[MAX_NUM_OF_EXPLOSIONS];
	m_pExplosionAnimatedSprite = new AnimatedSprite[MAX_NUM_OF_EXPLOSIONS];
}

Game::~Game()
{
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

	delete m_pPlayGameInMainMenuSprite;
	m_pPlayGameInMainMenuSprite = NULL;

	delete m_pQuitGameInMainMenuSprite;
	m_pQuitGameInMainMenuSprite = NULL;

	delete m_pSplashScreenSprite;
	m_pSplashScreenSprite = NULL;

	delete[] m_pExplosionAnimatedSprite;
	m_pExplosionAnimatedSprite = NULL;

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

	Mix_FreeChunk(m_pHurtSoundEffect);
	m_pHurtSoundEffect = NULL;

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

	delete m_pSplashScreen;
	m_pSplashScreen = NULL; 
	
	delete m_pMainMenu;
	m_pMainMenu = NULL;

	// Delete the backbuffer at the end.
	delete m_pBackBuffer;
	m_pBackBuffer = NULL;
}

bool
Game::Initialise()
{
	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	CreateBackBuffer();
	CreateInputHandler();
	CreateTTFFont();
	CreateFontColor(255, 255, 255);

	// Load sprite.
	m_pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	m_pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	m_pPlayerBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
	m_pEnemyBulletSprite = m_pBackBuffer->CreateSprite("assets\\enemybullet.png");
	m_pBackgroundSprite = m_pBackBuffer->CreateSprite("assets\\background.png");
	m_pInfoPanelSprite = m_pBackBuffer->CreateSprite("assets\\infopanel.png");
	m_pPlayGameInMainMenuSprite = m_pBackBuffer->CreateSprite("assets\\mainmenuplaygame.png");
	m_pQuitGameInMainMenuSprite = m_pBackBuffer->CreateSprite("assets\\mainmenuquitgame.png");
	m_pSplashScreenSprite = m_pBackBuffer->CreateSprite("assets\\splashscreen.png");

	// Set handle center.
	m_pPlayerSprite->SetHandleCenter();
	m_pEnemySprite->SetHandleCenter();
	m_pPlayerBulletSprite->SetHandleCenter();
	m_pEnemyBulletSprite->SetHandleCenter();

	// Load audio.
	m_pBackgroundMusic = Mix_LoadMUS("assets\\background.wav");
	m_pExplosionSoundEffect = Mix_LoadWAV("assets\\explosion.wav");
	m_pBulletSoundEffect = Mix_LoadWAV("assets\\bullet.wav");
	m_pHurtSoundEffect = Mix_LoadWAV("assets\\hurt.wav");

	// Create the main menu
	m_pSplashScreen = new SplashScreen();
	m_pSplashScreen->Initialise(m_pSplashScreenSprite);

	// Create the main menu
	m_pMainMenu = new MainMenu();
	m_pMainMenu->Initialise(m_pPlayGameInMainMenuSprite);

	// Create the player ship.
	SpawnPlayerShip();

	// Create the enemies.
	for (int i = 0; i < MAX_NUM_OF_ENEMY; i++)
	{
		int positionX = rand() % (WIDTH_OF_PLAYING_PANEL - m_pEnemySprite->GetWidth());
		int positionY = rand() % (LEVEL_TIME_DURATION * AVERAGE_VELOCITY_OF_ENEMY) * (-1);

		SpawnEnemy(positionX, positionY);
	}

	// Create the info panel.
	m_pInfoPanel = new InfoPanel();
	m_pInfoPanel->Initialise(m_pInfoPanelSprite);

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

void
Game::DoGameLoop()
{
	switch (sm_gameState)
	{
	case GAME_PLAY:
		DoGamePlayLoop(m_gamePlayLooping);
		break;
	case SPLASH_SCREEN:
		DoSplashScreenLoop(m_splashScreenLooping);
		break;
	case MAIN_MENU:
		DoMainMenuLoop(m_mainMenuLooping);
		break;
	}
}

void
Game::Process(float deltaTime)
{
	switch (sm_gameState)
	{
	case GAME_PLAY:
		ProcessGamePlay(deltaTime);
		break;
	case SPLASH_SCREEN:
		ProcessSplashScreen(deltaTime);
		break;
	case MAIN_MENU:
		ProcessMainMenu(deltaTime);
		break;
	}
}

void
Game::Draw(BackBuffer& backBuffer)
{
	switch (sm_gameState)
	{
	case GAME_PLAY:
		DrawGamePlay(backBuffer);
		break;
	case SPLASH_SCREEN:
		DrawSplashScreen(backBuffer);
		break;
	case MAIN_MENU:
		DrawMainMenu(backBuffer);
		break;
	}
}

void
Game::QuitGame()
{
	Game::sm_gameState = GAME_QUIT;

	m_splashScreenLooping = false;
	m_mainMenuLooping = false;
	m_gamePlayLooping = false;
}

void
Game::QuitSplashScreen()
{
	m_splashScreenLooping = false;
}

void
Game::QuitMainMenu()
{
	m_mainMenuLooping = false;
}

void
Game::QuitGamePlay()
{
	m_gamePlayLooping = false;
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
	if (!((m_pPlayerShip + m_indexOfPlayerShip - 1)->IsDead()))
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

		assert(bullet);

		if (bullet->Initialise(m_pPlayerBulletSprite))
		{
			float positionX = ((m_pPlayerShip + m_indexOfPlayerShip - 1)->GetPositionX() + (m_pPlayerSprite->GetWidth() - m_pPlayerBulletSprite->GetWidth()) / 2) * 1.0f;
			float positionY = ((m_pPlayerShip + m_indexOfPlayerShip - 1)->GetPositionY() - m_pPlayerBulletSprite->GetHeight()) * 1.0f;

			bullet->SetPositionX(positionX);
			bullet->SetPositionY(positionY);

			bullet->SetVerticalVelocity(VELOCITY_OF_PLAYER_BULLET * 1.0f);
		}

		// Play the bullet sound effect.
		Mix_PlayChannel(-1, m_pBulletSoundEffect, 0);
	}
}

void
Game::SpawnEnemy(int x, int y)
{
	Enemy* enemy = m_pEnemy;

	if (m_indexOfEnemy < MAX_NUM_OF_ENEMY)
	{
		enemy = m_pEnemy + m_indexOfEnemy;
		m_indexOfEnemy++;
	}
	else
	{
		m_indexOfEnemy = 0;
	}

	if (enemy->Initialise(m_pEnemySprite))
	{
		enemy->SetPosition(x * 1.0f, y * 1.0f);

		// To make the enemies to move towards the bottom of the screen, with a random velocity.
		if (rand() % 2 == 0) {
			enemy->SetVerticalVelocity(((rand() % STANDARD_DEVIATION_VELOCITY_OF_ENEMY) + AVERAGE_VELOCITY_OF_ENEMY) * 1.0f);
		}
		else
		{
			enemy->SetVerticalVelocity(((rand() % STANDARD_DEVIATION_VELOCITY_OF_ENEMY) * (-1) + AVERAGE_VELOCITY_OF_ENEMY) * 1.0f);
		}
	}
}

void
Game::SpawnPlayerShip()
{
	PlayerShip* playerShip = m_pPlayerShip;

	if (m_indexOfPlayerShip < MAX_NUM_OF_PLAYER_SHIP)
	{
		playerShip = m_pPlayerShip + m_indexOfPlayerShip;
		m_indexOfPlayerShip++;
	}

	if (playerShip->Initialise(m_pPlayerSprite))
	{
		float positionX = (Game::WIDTH_OF_PLAYING_PANEL - m_pPlayerBulletSprite->GetWidth()) / 2.0f;
		float positionY = (Game::HEIGHT_OF_PLAYING_PANEL - m_pPlayerBulletSprite->GetHeight()) * 1.0f;

		playerShip->SetPosition(positionX, positionY);
	}
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
		m_indexOfEnemyBullet = 0;
	}

	for (Enemy* enemy = m_pEnemy; enemy < m_pEnemy + MAX_NUM_OF_ENEMY; enemy++)
	{
		if (rand() % 100 > 90 && !(enemy->IsDead()) && enemy->GetPositionY() >= 0)
		{
			enemyBullet->Initialise(m_pEnemyBulletSprite);

			float positionX = (enemy->GetPositionX() + m_pEnemySprite->GetWidth() / 2) * 1.0f;
			float positionY = (enemy->GetPositionY() + m_pEnemySprite->GetHeight()) * 1.0f;

			enemyBullet->SetPositionX(positionX);
			enemyBullet->SetPositionY(positionY);

			// The ratio between the horizontal velocity and the vertical velocity of the enemy bullet.
			PlayerShip* playerShip = m_pPlayerShip + m_indexOfPlayerShip - 1;
			float r = abs((playerShip->GetPositionX() - enemyBullet->GetPositionX()) / (playerShip->GetPositionY() - enemyBullet->GetPositionY()));

			r = r > 3 ? 3 : r;

			if (playerShip->GetPositionY() < enemyBullet->GetPositionY() && playerShip->GetPositionX() < enemyBullet->GetPositionX())
			{
				enemyBullet->SetVerticalVelocity(VELOCITY_OF_ENEMY_BULLET * -1.0f);
				enemyBullet->SetHorizontalVelocity(VELOCITY_OF_ENEMY_BULLET * r * -1);
			}
			else if (playerShip->GetPositionY() < enemyBullet->GetPositionY() && playerShip->GetPositionX() > enemyBullet->GetPositionX())
			{
				enemyBullet->SetVerticalVelocity(VELOCITY_OF_ENEMY_BULLET * -1.0f);
				enemyBullet->SetHorizontalVelocity(VELOCITY_OF_ENEMY_BULLET * r);
			}
			else if (playerShip->GetPositionY() > enemyBullet->GetPositionY() && playerShip->GetPositionX() < enemyBullet->GetPositionX())
			{
				enemyBullet->SetVerticalVelocity(VELOCITY_OF_ENEMY_BULLET * 1.0f);
				enemyBullet->SetHorizontalVelocity(VELOCITY_OF_ENEMY_BULLET * r * -1);
			}
			else if (playerShip->GetPositionY() > enemyBullet->GetPositionY() && playerShip->GetPositionX() > enemyBullet->GetPositionX())
			{
				enemyBullet->SetVerticalVelocity(VELOCITY_OF_ENEMY_BULLET * 1.0f);
				enemyBullet->SetHorizontalVelocity(VELOCITY_OF_ENEMY_BULLET * r);
			}
		}
	}
}

void
Game::SpawnExplosion(float x, float y)
{
	Explosion* explosion = m_pExplosion;
	AnimatedSprite* explosionAnimatedSprite = m_pExplosionAnimatedSprite;

	if (m_indexOfExplosion < MAX_NUM_OF_EXPLOSIONS)
	{
		explosion = m_pExplosion + m_indexOfExplosion;
		m_indexOfExplosion++;
	}
	else
	{
		m_indexOfExplosion = 0;
	}

	if (m_indexOfExplosionAnimatedSprite < MAX_NUM_OF_EXPLOSIONS)
	{
		explosionAnimatedSprite = m_pExplosionAnimatedSprite + m_indexOfExplosionAnimatedSprite;
		m_indexOfExplosionAnimatedSprite++;
	}
	else
	{
		m_indexOfExplosionAnimatedSprite = 0;
	}

	explosionAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");

	// Each explosion has a separate explosion animated sprite.
	if (explosion->Initialise(explosionAnimatedSprite))
	{
		explosion->SetPositionX(x - WIDTH_OF_ANIMATED_SPRITE_FRAME / 2);
		explosion->SetPositionY(y - HEIGHT_OF_ANIMATED_SPRITE_FRAME / 2);
	}
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

void
Game::DoSplashScreenLoop(bool splashScreenLooping)
{
	GenericLoop(splashScreenLooping);
}

void
Game::DoMainMenuLoop(bool mainMenuLooping)
{
	GenericLoop(mainMenuLooping);
}

void
Game::DoGamePlayLoop(bool gamePlayLooping)
{
	GenericLoop(gamePlayLooping);
}

void
Game::ProcessSplashScreen(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessMainMenu(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessGamePlay(float deltaTime)
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
					float x = (enemy->GetPositionX() + bullet->GetPositionX() + m_pEnemySprite->GetCenterX() + m_pPlayerBulletSprite->GetCenterX()) / 2;
					float y = (enemy->GetPositionY() + bullet->GetPositionY() + m_pEnemySprite->GetCenterY() + m_pPlayerBulletSprite->GetCenterY()) / 2;

					// todo: change x, y
					SpawnExplosion(x, y);
					m_score += 10;

					enemy->SetDead(true);
					bullet->SetDead(true);

					// Play the explosion sound effect.
					Mix_PlayChannel(-1, m_pExplosionSoundEffect, 0);
				}
			}
		}
	}

	if (rand() % 100 > 92)
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
					// SpawnExplosion(x, y);

					// Play the hurt sound effect.
					Mix_PlayChannel(-1, m_pHurtSoundEffect, 0);
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
					float x = (enemy->GetPositionX() + playerShip->GetPositionX() + m_pEnemySprite->GetCenterX() + m_pPlayerBulletSprite->GetCenterX()) / 2;
					float y = (enemy->GetPositionY() + playerShip->GetPositionY() + m_pEnemySprite->GetCenterY() + m_pPlayerBulletSprite->GetCenterY()) / 2;

					UpdatePlayerShip(playerShip);
					enemy->SetDead(true);
					SpawnExplosion(x, y);

					// Play the hurt sound effect.
					Mix_PlayChannel(-1, m_pHurtSoundEffect, 0);
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
}

void
Game::DrawSplashScreen(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	m_pSplashScreen->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawMainMenu(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	m_pMainMenu->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawGamePlay(BackBuffer& backBuffer)
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

bool
Game::IsPlayGameMenuInMainMenuSelected()
{
	return m_playGameMenuInMainMenuSelected;
}

bool
Game::IsQuitGameMenuInMainMenuSelected()
{
	return m_quitGameMenuInMainMenuSelected;
}

void
Game::SelectPlayGameMenuInMainMenu()
{
	m_pMainMenu->SetSprite(m_pPlayGameInMainMenuSprite);

	m_playGameMenuInMainMenuSelected = true;
	m_quitGameMenuInMainMenuSelected = false;
}

void
Game::SelectQuitGameMenuInMainMenu()
{
	m_pMainMenu->SetSprite(m_pQuitGameInMainMenuSprite);

	m_playGameMenuInMainMenuSelected = false;
	m_quitGameMenuInMainMenuSelected = true;
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
		playerShip->SetDead(true);
		// Spawn explosion when the player ship dies.
		SpawnExplosion(playerShip->GetPositionX(), playerShip->GetPositionY());

		if (HasMoreLives())
		{
			--m_numOfLivesLeft;
			SpawnPlayerShip();
		}
		else
		{
			//QuitGamePlay();
			//Game::sm_gameState = GAME_SUMMARY;
		}
	}
	else
	{
		playerShip->SetHealth(playerShip->GetHealth() - DAMAGE_CAUSED_BY_ENEMY_BULLET);
	}
}

void
Game::UpdateElapsedSecondsAndFrameCount(float deltaTime)
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
}

void
Game::GenericLoop(bool looping)
{
	const float STEP_SIZE = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= STEP_SIZE)
		{
			Process(STEP_SIZE);

			m_lag -= STEP_SIZE;

			++m_numUpdates;
		}

		Draw(*m_pBackBuffer);
	}

	//	SDL_Delay(1);
}
