// COMP710 GP 2D Framework 2017
#ifndef __INFOPANEL_H__
#define __INFOPANEL_H__

#include "entity.h"

#include <string>

class InfoPanel : public Entity
{
	//Member Methods:
public:
	InfoPanel();
	~InfoPanel();

	bool Initialise(Sprite* sprite);
	void Process(float deltaTime);

protected:

private:
	InfoPanel(const InfoPanel& infoPanel);
	InfoPanel& operator=(const InfoPanel& infoPanel);

	//Member Data:
public:

protected:

private:

};

#endif //__INFOPANEL_H__
