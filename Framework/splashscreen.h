// COMP710 GP 2D Framework 2017
#ifndef __SPASHSCREEN_H__
#define __SPASHSCREEN_H__

// Local includes:
#include "entity.h"

class SplashScreen : public Entity
{
	//Member Methods:
public:
	SplashScreen();
	~SplashScreen();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

	// Set the sprite for the splash screen based on the user selection.
	void SetSprite(Sprite* sprite);

protected:

private:
	SplashScreen(const SplashScreen& splashScreen);
	SplashScreen& operator=(const SplashScreen& splashScreen);

	//Member Data:
public:

protected:

private:

};

#endif //__SPASHSCREEN_H__
