// COMP710 GP 2D Framework 2017
#ifndef __TEXTTEXTURE_H__
#define __TEXTTEXTURE_H__

// Library includes:
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

//Texture wrapper class
class TextTexture
{
public:
	//Initializes variables
	TextTexture();

	//Deallocates memory
	~TextTexture();

	bool Initialise(TTF_Font* font, SDL_Renderer* renderer);

	//Creates image from font string
	bool LoadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void Free();

	//Set color modulation
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void SetBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void SetAlpha(Uint8 alpha);

	//Renders texture at given point
	void Render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int GetWidth();
	int GetHeight();

private:
	SDL_Renderer* m_pRenderer;
	TTF_Font* m_pFont;

	//The actual hardware texture
	SDL_Texture* m_pTexture;

	//Image dimensions
	int m_width;
	int m_height;
};

#endif // __TEXTTEXTURE_H__
