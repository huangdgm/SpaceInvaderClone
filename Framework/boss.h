// COMP710 GP 2D Framework 2017
#ifndef __BOSS_H__
#define __BOSS_H__

#include "enemy.h"
#include "weapontype.h"

class Boss : public Enemy
{
	//Member Methods:
public:
	Boss();
	~Boss();

	bool Initialise(Sprite* sprite);
	void Process(float deltaTime);

	void SetHealth(int health);
	int GetHealth() const;

protected:

private:
	Boss(const Boss& boss);
	Boss& operator=(const Boss& boss);

	//Member Data:
public:
	int m_health;
	WeaponType m_weaponType;

protected:

private:

};

#endif //__BOSS_H__