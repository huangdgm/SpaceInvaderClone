
// This include:
#include "texttexture.h"

// Local includes:
#include "logmanager.h"

// Library includes:
#include <SDL_ttf.h>
#include <cassert>

TextTexture::TextTexture()
: m_pRenderer(NULL)
, m_pFont(NULL)
, m_pTexture(NULL)
, m_width(0)
, m_height(0)
{

}

TextTexture::~TextTexture()
{
	//Deallocate
	Free();
}

bool
TextTexture::Initialise(TTF_Font* font, SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
	m_pFont = font;

	assert(font);
	assert(renderer);

	return (true);
}


// With loadFromRenderedText(), we can easily create a texture with the content of the textureText.
bool TextTexture::LoadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	Free();

	// Render text surface
	// Create SDL_Surface from TTF_Font, char* and SDL_Color.
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_pFont, textureText.c_str(), textColor);

	if (textSurface == NULL)
	{
		LogManager::GetInstance().Log("Unable to render text surface!");
	}
	else
	{
		//Create texture from surface pixels
		m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, textSurface);

		if (m_pTexture == NULL)
		{
			LogManager::GetInstance().Log("Unable to create texture from rendered text!");
		}
		else
		{
			//Get image dimensions
			m_width = textSurface->w;
			m_height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return m_pTexture != NULL;
}

void TextTexture::Free()
{
	//Free texture if it exists
	if (m_pTexture != NULL)
	{
		SDL_DestroyTexture(m_pTexture);
		m_pTexture = NULL;

		m_width = 0;
		m_height = 0;
	}
}

void TextTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(m_pTexture, red, green, blue);
}

void TextTexture::SetBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(m_pTexture, blending);
}

void TextTexture::SetAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(m_pTexture, alpha);
}

void TextTexture::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(m_pRenderer, m_pTexture, clip, &renderQuad, angle, center, flip);
}

int TextTexture::GetWidth()
{
	return m_width;
}

int TextTexture::GetHeight()
{
	return m_height;
}