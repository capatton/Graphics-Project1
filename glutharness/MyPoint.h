#include <stdlib.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include "windows/glut/glut.h"
#else
#include <GL/glut.h>
#endif

class MyPoint {
private:
	GLfloat x;
	GLfloat y;
public:
	MyPoint() 
	: x(0), y(0)
	{
	}
	MyPoint(GLfloat x, GLfloat y) 
	: x(x), y(y)
	{
	}

	MyPoint operator+(MyPoint other) {
		return MyPoint(this->x + other.x, this->y + other.y);
	}

	MyPoint operator/(GLfloat divisor) {
		return MyPoint(this->x / divisor, this->y / divisor);
	}
};