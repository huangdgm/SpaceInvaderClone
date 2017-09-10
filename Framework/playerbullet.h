// COMP710 GP 2D Framework 2017
#ifndef __PLAYERBULLET_H__
#define __PLAYERBULLET_H__

#include "entity.h"

class PlayerBullet : public Entity
{
	//Member Methods:
public:
	PlayerBullet();
	~PlayerBullet();

	bool Initialise(Sprite* sprite);
	void Process(float deltaTime);

protected:

private:
	PlayerBullet(const PlayerBullet& bullet);
	PlayerBullet& operator=(const PlayerBullet& bullet);

	//Member Data:
public:

protected:

private:

};

#endif //__PLAYERBULLET_H__