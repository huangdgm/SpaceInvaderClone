// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

// Local includes:
#include "enemy.h"
#include "playerbullet.h"
#include "enemybullet.h"
#include "animatedsprite.h"
#include "explosion.h"
#include "texture.h"
#include "texttexture.h"

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
	bool DoGameLoop();
	void Quit();

	void StopSpaceShip();
	void MoveSpaceShipLeft();
	void MoveSpaceShipRight();
	void MoveSpaceShipUp();
	void MoveSpaceShipDown();
	void FireSpaceShipBullet();

	void SpawnEnemy(int x, int y);
	void SpawnPlayerShip();
	void SpawnEnemyBullet();
	void SpawnExplosion(float x, float y);

	bool HasMoreLives();
	void UpdatePlayerShip(PlayerShip* playerShip);

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool CreateBackBuffer();
	bool CreateInputHandler();
	bool CreateTTFFont();
	bool CreateFontColor(Uint8 r, Uint8 g, Uint8 b);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:
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
	bool m_looping;

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
	Sprite* m_pPlayerSprite;
	Sprite* m_pPlayerBulletSprite;
	Sprite* m_pEnemyBulletSprite;
	Sprite* m_pBackgroundSprite;
	Sprite* m_pInfoPanelSprite;

	PlayerShip* m_pPlayerShip;
	Enemy* m_pEnemy;
	PlayerBullet* m_pPlayerBullet;
	EnemyBullet* m_pEnemyBullet;
	Explosion* m_pExplosion;
	InfoPanel* m_pInfoPanel;

	const static int MAX_NUM_OF_ENEMY = 10;
	const static int MAX_NUM_OF_PLAYER_SHIP = 3;

	const static int MAX_NUM_OF_PLAYER_BULLETS = 30;
	const static int MAX_NUM_OF_ENEMY_BULLETS = 30;
	const static int MAX_NUM_OF_EXPLOSIONS = 56;
	const static int MAX_NUM_OF_ANIMATEDSPRITE = 56;

	const static int VELOCITY_OF_PLAYER_BULLET = -800;
	const static int VELOCITY_OF_ENEMY_BULLET = 100;
	const static int VELOCITY_OF_PLAYERSHIP = 400;
	const static int VELOCITY_OF_ENEMY = 50;

	const static int DAMAGE_CAUSED_BY_ENEMY_BULLET = 34;

	int m_indexOfPlayerBullet;
	int m_indexOfEnemyBullet;
	int m_indexOfEnemy;
	int m_indexOfPlayerShip;
	int m_indexOfExplosion;

	int m_scrollingOffset;

	// Background music.
	Mix_Music* m_pBackgroundMusic;

	// The sound effects.
	Mix_Chunk* m_pExplosionSoundEffect;
	Mix_Chunk* m_pBulletSoundEffect;

	TTF_Font* m_pFont;

	// The color for the font.
	SDL_Color* m_pFontColor;

	// The score text texture.
	TextTexture* m_pScoreTextTexture;
	TextTexture* m_pLevelTextTexture;
	TextTexture* m_pLivesTextTexture;
	TextTexture* m_pHealthTextTexture;

	int m_level;
	int m_score;
	int m_numOfLivesLeft;

private:

};

#endif // __GAME_H__
