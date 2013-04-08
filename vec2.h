#include <iostream>
#include <GL/glut.h>

class vec2 
{
private:
	GLfloat x;
	GLfloat y;
public:
	vec2() 
	: x(0), y(0)
	{
	}

	vec2(GLfloat x, GLfloat y)
	: x(x), y(y)
	{
	}

	vec2 operator+(vec2 RHS)
	{
		return vec2(this->x + RHS.x, this->y + RHS.y);
	}

	vec2 operator/(GLfloat divisor)
	{
		return vec2(this->x / divisor, this->y / divisor);
	}

};

