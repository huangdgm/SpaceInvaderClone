// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

// Local includes:
#include "enemy.h"
#include "bullet.h"
#include "animatedsprite.h"
#include "explosion.h"
#include "texture.h"

#include <SDL_mixer.h>

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class PlayerShip;

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
	void SpawnExplosion(float x, float y);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:
	const static int m_widthOfWindow = 800;
	const static int m_heightOfWindow = 600;

	const static int m_widthOfAnimatedSpriteFrame = 64;
	const static int m_heigthOfAnimatedSpriteFrame = 64;

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
	Sprite* m_pBackgroundSprite;

	AnimatedSprite* m_AnimatedSpriteArray[56];

	PlayerShip* m_pPlayerShip;
	Enemy* m_enemy2DArray[4][14];
	Bullet* m_bulletArray[30];
	Explosion* m_explosionArray[56];

	const static int m_numOfEnemyRows = 4;
	const static int m_numOfEnemyCols = 14;

	const static int m_maxNumOfBullets = 30;
	const static int m_maxNumOfExplosions = 56;
	const static int m_maxNumOfAnimatedSprite = 56;

	const static int m_velocityOfBullet = -800;
	const static int m_velocityOfPlayerShip = 400;

	int m_indexOfBullet;
	int m_indexOfExplosion;
	int m_indexOfAnimatedSprite;

	int m_scrollingOffset;

	// Background music.
	Mix_Music* m_pBackgroundMusic;

	// The sound effects.
	Mix_Chunk* m_pExplosionSoundEffect;
	Mix_Chunk* m_pBulletSoundEffect;

private:

};

#endif // __GAME_H__
