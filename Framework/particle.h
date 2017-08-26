#ifndef __PARTICLE_H__
#define __PARTICLE_H__

class Vector2D;
class BackBuffer;

class Particle
{
	//Member Methods:
public:
	Particle();
	~Particle();

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

protected:

private:
	Particle(const Particle& explosion);
	Particle& operator=(const Particle& explosion);

	//Member Data:
public:

protected:
	Vector2D* m_position;
	Vector2D* m_velocity;
	Vector2D* m_acceleration;

	float m_age;

private:

};

#endif // __PARTICLE_H__