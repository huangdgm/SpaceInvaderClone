// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"
#include "game.h"

AnimatedSprite::AnimatedSprite()
: m_frameSpeed(0.0f)
, m_frameWidth(0)
, m_timeElapsed(0.0f)
, m_currentFrame(0)
, m_paused(false)
, m_loop(false)
, m_animating(false)
{

}

AnimatedSprite::~AnimatedSprite()
{

}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	m_frameWidth = 0;
	m_frameSpeed = 0.06f;

	m_loop = false;
	m_paused = false;
	m_animating = true;

	Sprite::Initialise(texture);

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrame(int x)
{
	// Add the x coordinate to the frame coordinate array.
	m_frameCoordinatesArray[m_currentFrame++] = x;
}

void
AnimatedSprite::Process(float deltaTime)
{
	if (m_animating)
	{
		if (!m_paused)
		{
			m_timeElapsed += deltaTime;
		}

		// If the time elapsed is greater than the frame speed.
		if (m_timeElapsed > m_frameSpeed)
		{
			// Move to the next frame.
			// Reset the time elapsed counter.
			m_currentFrame++;
			m_timeElapsed = 0;
		}

		// If the current frame is greater than the number of frame in this animation...
		// Reset to the first frame.
		if (m_currentFrame > 4)
		{
			m_currentFrame = 0;

			// Stop the animation if it is not looping...
			if (!m_loop)
			{
				m_animating = false;
			}
		}
	}
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	// Draw the particular frame into the backbuffer.
	backbuffer.DrawAnimatedSprite(*this);
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void
AnimatedSprite::SetFrameWidth(int w)
{
	m_frameWidth = w;
}

void
AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}

int
AnimatedSprite::GetCurrentFrame()
{
	return m_currentFrame;
}

void
AnimatedSprite::ResetCurrentFrame()
{
	m_currentFrame = 0;
}

int
AnimatedSprite::GetFrameCoordinate(int currentFrame)
{
	return m_frameCoordinatesArray[currentFrame];
}