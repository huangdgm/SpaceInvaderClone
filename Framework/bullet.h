// COMP710 GP 2D Framework 2017
#ifndef __BULLET_H__
#define __BULLET_H__

#include "entity.h"

class Bullet : public Entity
{
	//Member Methods:
public:
	Bullet();
	~Bullet();
	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

protected:

private:
	Bullet(const Bullet& bullet);
	Bullet& operator=(const Bullet& bullet);

	//Member Data:
public:

protected:

private:

};

#endif //__BULLET_H__