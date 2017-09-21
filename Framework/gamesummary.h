// COMP710 GP 2D Framework 2017
#ifndef __GAMESUMMARY_H__
#define __GAMESUMMARY_H__

// Local includes:
#include "entity.h"
#include "SDL.h"

class TextTexture;

class GameSummary : public Entity
{
	//Member Methods:
public:
	GameSummary();
	~GameSummary();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

	void SetContent(int score, int health, int numOfEnemy, int numOfBoss);

protected:

private:
	GameSummary(const GameSummary& gameSummary);
	GameSummary& operator=(const GameSummary& gameSummary);

	//Member Data:
public:

protected:
	SDL_Color* m_pFontColor;

	TextTexture* m_pScoreTextTexture;
	TextTexture* m_pHealthTextTexture;
	TextTexture* m_pNumberOfEnemyTextTexture;
	TextTexture* m_pBossTextTexture;

	int m_score;
	int m_health;
	int m_numOfEnemy;
	int m_numOfBoss;

private:

};

#endif //__GAMESUMMARY_H__
