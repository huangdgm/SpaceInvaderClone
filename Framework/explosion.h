// COMP710 GP 2D Framework 2017
#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

// Forward Declarations:
class BackBuffer;
class Texture;
class AnimatedSprite;

class Explosion
{
	//Member Methods:
public:
	Explosion();
	~Explosion();

	bool Initialise(AnimatedSprite* animatedSprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetPositionX(float x);
	void SetPositionY(float y);

	float GetPositionX() const;
	float GetPositionY() const;

	void SetDead(bool dead);
	bool IsDead() const;

	bool IsExplosing() const;

protected:

private:
	Explosion(const Explosion& explosion);
	Explosion& operator=(const Explosion& explosion);

	//Member Data:
public:

protected:
	AnimatedSprite* m_pAnimatedSprite;

	// Position
	float m_x;
	float m_y;

	bool m_dead;

private:

};

#endif // __EXPLOSION_H__