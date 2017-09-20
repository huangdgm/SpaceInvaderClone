// COMP710 GP 2D Framework 2017
#ifndef __GAMESUMMARY_H__
#define __GAMESUMMARY_H__

// Local includes:
#include "entity.h"

class TextTexture;

class GameSummary : public Entity
{
	//Member Methods:
public:
	GameSummary();
	~GameSummary();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

protected:

private:
	GameSummary(const GameSummary& gameSummary);
	GameSummary& operator=(const GameSummary& gameSummary);

	//Member Data:
public:

protected:
	TextTexture* m_pScoreTextTexture;
	TextTexture* m_pHealthTextTexture;
	TextTexture* m_pNumberOfEnemyTextTexture;
	TextTexture* m_pBossTextTexture;

private:

};

#endif //__GAMESUMMARY_H__
