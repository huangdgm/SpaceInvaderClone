// COMP710 GP 2D Framework 2017

// This include:
#include "gamesummary.h"

// Local includes:
#include "texttexture.h"
#include "game.h"

// Library includes:

GameSummary::GameSummary()
: Entity()
, m_pFontColor(0)
, m_pScoreTextTexture(0)
, m_pHealthTextTexture(0)
, m_pNumberOfEnemyTextTexture(0)
, m_pBossTextTexture(0)
, m_score(0)
, m_health(0)
, m_numOfEnemy(0)
, m_numOfBoss(0)
{

}

GameSummary::~GameSummary()
{

}

void
GameSummary::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	this->SetPosition(0, 0);
}

void
GameSummary::SetContent(int score, int health, int numOfEnemy, int numOfBoss)
{
	m_score = score;
	m_health = health;
	m_numOfEnemy = numOfEnemy;
	m_numOfBoss = numOfBoss;
}

//void
//GameSummary::Draw(BackBuffer& backBuffer)
//{
//	
//
//	m_pScoreTextTexture->LoadFromRenderedText(std::to_string(m_score), *m_pFontColor);
//	m_pHealthTextTexture->LoadFromRenderedText(std::to_string(m_health), *m_pFontColor);
//	m_pNumberOfEnemyTextTexture->LoadFromRenderedText(std::to_string(m_numOfEnemy), *m_pFontColor);
//	m_pBossTextTexture->LoadFromRenderedText(std::to_string(m_numOfBoss), *m_pFontColor);
//
//	m_pScoreTextTexture->Render(Game::SCORE_POSITION_X, Game::SCORE_POSITION_Y);
//	m_pHealthTextTexture->Render(Game::LEVEL_POSITION_X, Game::LEVEL_POSITION_Y);
//	m_pNumberOfEnemyTextTexture->Render(Game::LIVES_POSITION_X, Game::LIVES_POSITION_Y);
//	m_pBossTextTexture->Render(Game::HEALTH_POSITION_X, Game::HEALTH_POSITION_Y);
//
//	Entity::Draw(backBuffer);
//}
