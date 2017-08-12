// COMP710 GP 2D Framework 2017
#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

#include "entity.h"

class PlayerShip : public Entity
{
	//Member Methods:
public:
	PlayerShip();
	~PlayerShip();	// Make it virtual as well, as it is virtual in base class.
	void Process(float deltaTime);	// Basically, declare the method virtual as well, although it is not necessary to do so.
	void Initialise(Sprite* sprite);

protected:

private:
	PlayerShip(const PlayerShip& playerShip);
	PlayerShip& operator=(const PlayerShip& playerShip);

	//Member Data:
public:

protected:

private:

};

#endif //__PLAYERSHIP_H__
