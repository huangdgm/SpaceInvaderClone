// COMP710 GP 2D Framework 2017
#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

#include "entity.h"

class PlayerShip : public Entity
{
	//Member Methods:
public:
	PlayerShip();
	~PlayerShip();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

	void SetScore(int score);
	void SetHealth(int health);
	void SetNumOfLivesLeft(int numOfLivesLeft);

	int GetScore() const;
	int GetHealth() const;
	int GetNumOfLivesLeft() const;

protected:

private:
	PlayerShip(const PlayerShip& playerShip);
	PlayerShip& operator=(const PlayerShip& playerShip);

	//Member Data:
public:
	int m_score;
	int m_health;
	int m_numOfLivesLeft;

protected:

private:

};

#endif //__PLAYERSHIP_H__
