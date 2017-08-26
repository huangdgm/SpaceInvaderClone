#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

//Member Methods:
class Vector2D
{
public:
	Vector2D();
	~Vector2D();

	float getLength() const;

	static float dot(Vector2D v1, Vector2D v2);
	static Vector2D proj(const Vector2D& vector2D);

protected:

private:
	Vector2D(const Vector2D& vector2D);
	Vector2D& operator=(const Vector2D& vector2D);

	//Member Data:
public:

protected:
	float m_x;
	float m_y;

private:
};

#endif // __VECTOR2D_H__