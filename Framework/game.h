// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

// Local includes:
#include "enemy.h"
#include "boss.h"
#include "playerbullet.h"
#include "enemybullet.h"
#include "animatedsprite.h"
#include "explosion.h"
#include "texture.h"
#include "texttexture.h"
#include "gamestate.h"
#include "mainmenu.h"
#include "splashscreen.h"
#include "gamesummary.h"

#include <SDL_ttf.h>
#include <SDL_mixer.h>

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class PlayerShip;
class InfoPanel;

using namespace std;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	void DoGameLoop();
	void GenericLoop(bool looping);

	void QuitGame();
	void QuitSplashScreen();
	void QuitMainMenu();
	void QuitGamePlay();
	void QuitGameSummary();

	void StopSpaceShip();
	void MoveSpaceShipLeft();
	void MoveSpaceShipRight();
	void MoveSpaceShipUp();
	void MoveSpaceShipDown();
	void FireSpaceShipBullet();

	void SpawnEnemy(int x, int y);
	void SpawnPlayerShip();
	void SpawnEnemyBullet();
	void SpawnBossBullet();
	void SpawnExplosion(float x, float y);

	bool HasMoreLives();
	void UpdatePlayerShip(PlayerShip* playerShip);

	bool IsPlayGameMenuInMainMenuSelected();
	bool IsQuitGameMenuInMainMenuSelected();

	void SelectPlayGameMenuInMainMenu();
	void SelectQuitGameMenuInMainMenu();

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	Game();

	void ResetGamePlay();
	void InitialiseGamePlay();

	void UpdateElapsedSecondsAndFrameCount(float deltaTime);

	bool CreateBackBuffer();
	bool CreateInputHandler();
	bool CreateTTFFont();
	bool CreateFontColor(Uint8 r, Uint8 g, Uint8 b);

	void DoGamePlayLoop(bool gamePlayLooping);
	void DoSplashScreenLoop(bool mainMenuLooping);
	void DoMainMenuLoop(bool splashScreenLooping);
	void DoGameSummaryLoop(bool gameSummaryLooping);
	//bool DoPausedMenuLoop();

	void ProcessGamePlay(float deltaTime);
	void ProcessSplashScreen(float deltaTime);
	void ProcessMainMenu(float deltaTime);
	void ProcessGameSummary(float deltaTime);
	//void ProcessPausedMenu(float deltaTime);

	void DrawGamePlay(BackBuffer& backBuffer);
	void DrawSplashScreen(BackBuffer& backBuffer);
	void DrawMainMenu(BackBuffer& backBuffer);
	void DrawGameSummary(BackBuffer& backBuffer);
	//void DrawPausedMenu(BackBuffer& backBuffer);

	//Member Data:
public:
	static GameState sm_gameState;

	const static int WIDTH_OF_WINDOW = 800;
	const static int HEIGHT_OF_WINDOW = 800;

	const static int WIDTH_OF_PLAYING_PANEL = 600;
	const static int HEIGHT_OF_PLAYING_PANEL = 800;

	const static int WIDTH_OF_INFO_PANEL = 200;
	const static int HEIGHT_OF_INFO_PANEL = 800;

	const static int WIDTH_OF_ANIMATED_SPRITE_FRAME = 64;
	const static int HEIGHT_OF_ANIMATED_SPRITE_FRAME = 64;

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;

	bool m_splashScreenLooping;
	bool m_mainMenuLooping;
	bool m_gamePlayLooping;
	bool m_gameSummaryLooping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	Sprite* m_pEnemySprite;
	Sprite* m_pBossSprite;
	Sprite* m_pPlayerSprite;
	Sprite* m_pPlayerBulletSprite;
	Sprite* m_pEnemyBulletSprite;
	Sprite* m_pBackgroundSprite;
	Sprite* m_pInfoPanelSprite;
	Sprite* m_pPlayGameInMainMenuSprite;
	Sprite* m_pQuitGameInMainMenuSprite;
	Sprite* m_pSplashScreenSprite;
	Sprite* m_pGameSummarySprite;

	AnimatedSprite* m_pExplosionAnimatedSprite;

	PlayerShip*		m_pPlayerShip;
	Enemy*			m_pEnemy;
	Boss*			m_pBoss;
	PlayerBullet*	m_pPlayerBullet;
	EnemyBullet*	m_pEnemyBullet;
	Explosion*		m_pExplosion;
	InfoPanel*		m_pInfoPanel;
	SplashScreen*	m_pSplashScreen;
	MainMenu*		m_pMainMenu;
	GameSummary*	m_pGameSummary;

	// Configuration of the game.
	const static int MAX_NUM_OF_ENEMY = 100;
	const static int MAX_NUM_OF_PLAYER_SHIP = 3;

	const static int MAX_NUM_OF_PLAYER_BULLETS = 50;
	const static int MAX_NUM_OF_ENEMY_BULLETS = 100;
	const static int MAX_NUM_OF_EXPLOSIONS = 100;
	const static int MAX_NUM_OF_ANIMATEDSPRITE = 100;

	const static int VELOCITY_OF_PLAYER_BULLET = -800;
	const static int VELOCITY_OF_ENEMY_BULLET = 200;
	const static int VELOCITY_OF_PLAYERSHIP = 400;
	const static int VELOCITY_OF_BOSS = 100;
	const static int AVERAGE_VELOCITY_OF_ENEMY = 120;
	const static int STANDARD_DEVIATION_VELOCITY_OF_ENEMY = 40;

	const static int DAMAGE_CAUSED_BY_ENEMY_BULLET = 34;
	const static int DAMAGE_CAUSED_BY_PLAYERSHIP_BULLET = 20;
	const static int LEVEL_TIME_DURATION = 30;

	const static int SCORE_POSITION_X = 690;
	const static int SCORE_POSITION_Y = 60;
	const static int LEVEL_POSITION_X = 700;
	const static int LEVEL_POSITION_Y = 250;
	const static int LIVES_POSITION_X = 736;
	const static int LIVES_POSITION_Y = 410;
	const static int HEALTH_POSITION_X = 665;
	const static int HEALTH_POSITION_Y = 580;

	int m_indexOfPlayerBullet;
	int m_indexOfEnemyBullet;
	int m_indexOfEnemy;
	int m_indexOfPlayerShip;
	int m_indexOfExplosion;
	int m_indexOfExplosionAnimatedSprite;

	// The offset for the scrolling background.
	int m_scrollingOffset;

	// Background music.
	Mix_Music* m_pBackgroundMusic;

	// The sound effects.
	Mix_Chunk* m_pExplosionSoundEffect;
	Mix_Chunk* m_pBulletSoundEffect;
	Mix_Chunk* m_pHurtSoundEffect;

	TTF_Font* m_pFont;

	SDL_Color* m_pFontColor;

	// The text texture.
	TextTexture* m_pScoreTextTexture;
	TextTexture* m_pLevelTextTexture;
	TextTexture* m_pLivesTextTexture;
	TextTexture* m_pHealthTextTexture;

	// Information shown in the info panel.
	int m_level;
	int m_score;
	int m_numOfLivesLeft;

	bool m_playGameMenuInMainMenuSelected;
	bool m_quitGameMenuInMainMenuSelected;

private:

};

#endif // __GAME_H__
