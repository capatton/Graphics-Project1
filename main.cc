#include <GL/glut.h>
#include "vec2.h"
#include <stdio.h>

void handleKeyPress(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
			exit(0);
	}
}

int main(int argc, char **argv)
{
	printf("Hello world");
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE);
	//glutInitWindowSize(500,500);
	//glutInitWindowPosition(100,100);
	glutCreateWindow("Caseys Window");


	const int NumPoints = 5000;
	vec2 points[NumPoints];
	// A triangle in the plane z= 0
	vec2 vertices[3]={vec2(-0.5,-0.5), vec2(0.5,-0.5),
	vec2(0.0, 0.5)};
	// An arbitrary initial point inside the triangle
	points[0] = vec2(0.25, 0.25);
	// compute and store NumPoints-1 new points
	for(int k = 1; k < NumPoints; k++)
	{
		int j = rand() % 3; // pick a vertex at random
		// Compute the point halfway between selected
		// vertex and previous point
		points[k] = (points[k-1]+vertices[j])/2.0;
	}

	glutKeyboardFunc(handleKeyPress);

	glutMainLoop();

	return 0;
}