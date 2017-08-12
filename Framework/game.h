// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

// Local includes:
#include "enemy.h"
#include "bullet.h"

// Library includes:
#include <vector>
// #include <list>

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
	void MoveSpaceShipRight();	// move spaceship right
	void FireSpaceShipBullet();

	void SpawnEnemy(int x, int y);
	void SpawnExplosion(int x, int y);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

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

	// Game Entities:
	// W03.1: Add a PlayerShip field.
	PlayerShip* m_pPlayerShip;
	// W03.2: Add an alien enemy container field.
	vector<Enemy*> enemyContainer;
	// list<Enemy*> enemyContainer;
	// int*[] enemyArray;
	// W03.3: Add a bullet container field.
	vector<Bullet*> bulletContainer;
	// list<Bullet*> bulletContainer;

private:

};

#endif // __GAME_H__
