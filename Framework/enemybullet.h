// COMP710 GP 2D Framework 2017
#ifndef __ENEMYBULLET_H__
#define __ENEMYBULLET_H__

#include "entity.h"

class EnemyBullet : public Entity
{
	//Member Methods:
public:
	EnemyBullet();
	~EnemyBullet();

	bool Initialise(Sprite* sprite);
	void Process(float deltaTime);

protected:

private:
	EnemyBullet(const EnemyBullet& bullet);
	EnemyBullet& operator=(const EnemyBullet& bullet);

	//Member Data:
public:

protected:

private:

};

#endif //__ENEMYBULLET_H__