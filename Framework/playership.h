// COMP710 GP 2D Framework 2017
#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

// Local includes:
#include "entity.h"
#include "weapontype.h"

class PlayerShip : public Entity
{
	//Member Methods:
public:
	PlayerShip();
	~PlayerShip();

	void Process(float deltaTime);
	bool Initialise(Sprite* sprite);

	void SetHealth(int health);
	void SetNumOfLivesLeft(int numOfLivesLeft);

	int GetHealth() const;

protected:

private:
	PlayerShip(const PlayerShip& playerShip);
	PlayerShip& operator=(const PlayerShip& playerShip);

	//Member Data:
public:
	int m_health;
	WeaponType m_weaponType;

protected:

private:

};

#endif //__PLAYERSHIP_H__
