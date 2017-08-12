// COMP710 GP 2D Framework 2017
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

class Enemy : public Entity
{
	//Member Methods:
public:
	Enemy();
	~Enemy();
	void Process(float deltaTime);

protected:

private:
	Enemy(const Enemy& enemy);
	Enemy& operator=(const Enemy& enemy);

	//Member Data:
public:

protected:

private:

};

#endif //__ENEMY_H__
