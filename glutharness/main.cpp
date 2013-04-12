#include <GL/glew.h>
#include <GL/glut.h>
#include "vec.h"
#include "Angel.h"
#include "InitShader.cpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;

typedef vec2 point;

typedef struct RGBAColor {
    GLubyte r, g, b, a;
} RGBAColor;

enum colors {
    RED,
    GREEN,
    BLUE
};

int colorToShow = 0;


const unsigned int numPoints = 4;//was 4
const unsigned int numDivisions = 5;

namespace Global
{
    GLint Ndivisions = numDivisions;
    GLint Nvertices = (GLint) pow(3.0,Ndivisions + 1);
    vector<vec2> points;
};

void triangle( vec2 a, vec2 b, vec2 c, vector<vec2> &points )
{
    size_t old_size = points.size();
    points.resize(old_size + 3);
    points[old_size] = a;
    points[old_size + 1] = b;
    points[old_size + 2] = c;
}

vec2 divide( vec2 vector, double scalar )
{
    return vec2(vector.x/scalar,vector.y/scalar);
}

void divide_triangle( vec2 a, vec2 b, vec2 c, GLint k, vector<vec2> &points )
{
    if(k>0)
    {
        vec2 ab = divide(a + b,2.0);
        vec2 ac = divide(a + c,2.0);
        vec2 bc = divide(b + c,2.0);

        //Divide the triangle recursively
        int new_k = k - 1;
        divide_triangle(a,ab,ac,new_k,points);
        divide_triangle(c,ac,bc,new_k,points);
        divide_triangle(b,bc,ab,new_k,points);
    }
    else
    {
        triangle(a,b,c,points);
    }
}

GLuint prog = 0;
GLuint vbo = 0;
void init()
{
    glewInit();
    prog = InitShader( "../vsource.glsl", "../fsource.glsl" );
    
    //Init the global array
    divide_triangle
        (
        vec2(-0.5, -0.5),
        vec2(0.5,-0.5),
        vec2(0.0,0.5),
        Global::Ndivisions,
        Global::points
        );

    // create/fill VBO
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    unsigned int numBytes = sizeof( vec2 ) * Global::points.size();
    glBufferData( GL_ARRAY_BUFFER, numBytes, &Global::points[0].x, GL_STATIC_DRAW );
}

void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT );

    if (colorToShow == RED){
        glColor3f(1.0f,0.0f,0.0f);      
    }
    else if (colorToShow == GREEN) {
        glColor3f(0.0f,1.0f,0.0f);      
    }
    else if (colorToShow == BLUE) {
        glColor3f(0.0f,0.0f,1.0f);      

    }

    glUseProgram( prog );

    GLint position_loc = glGetAttribLocation( prog, "position" );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glVertexAttribPointer( position_loc, 2, GL_FLOAT, GL_FALSE, sizeof( vec2 ), 0 );
    glEnableVertexAttribArray( position_loc );

    glDrawArrays( GL_POINTS, 0, Global::points.size() );

    glDisableVertexAttribArray( position_loc );

    glutSwapBuffers();
}

void keyPressHandler(unsigned char key, int x, int y)
{
    switch (key){
        case 'Q':
        case 'q':
            exit(0);
        case 'C':
        case 'c':
            ++colorToShow;
            colorToShow %= 3;
            display();
            break;
    }
}


int main(int argc, char **argv)
{
    glutInit( &argc, argv );
    glutInitWindowSize( 500,600 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutCreateWindow( "Sierpinski gasket" );
    init();
    glutDisplayFunc( display );
    glutKeyboardFunc(keyPressHandler);
    glutMainLoop();
    return 0;
}