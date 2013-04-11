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
// vertex shader
// const GLchar* vert =
// "#version 120\n"
// "attribute vec2 position;"
// "void main()"
// "{"
// "   gl_Position = vec4( position, 0.0, 1.0 );"
// "}"
// ;

// fragment shader
// const GLchar* frag = 
// "#version 120\n"
// "void main()"
// "{"
// "    gl_FragColor = vec4( 0.0, 1.0, 0.0, 1.0 );"
// "}"
// ;

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

void CheckStatus( const GLenum id )
{
    GLint status = GL_FALSE, loglen = 10;
    if( glIsShader(id) ) {
        glGetShaderiv( id, GL_COMPILE_STATUS, &status );
    } 
    if( glIsProgram(id) )  {
        glGetProgramiv( id, GL_LINK_STATUS, &status );
    }

    if( GL_TRUE == status ) {
        return;
    }
    if( glIsShader(id) ) {
        glGetShaderiv( id, GL_INFO_LOG_LENGTH , &loglen);
    } 
    if( glIsProgram(id) ) { 
        glGetProgramiv( id, GL_INFO_LOG_LENGTH , &loglen);
    }
    vector< char > log( loglen, 'E' );
    if( glIsShader(id) )  { 
        glGetShaderInfoLog( id, loglen, NULL, &log[0] );
    }
    if( glIsProgram(id) ) { 
        glGetProgramInfoLog( id, loglen, NULL, &log[0] );
    }

    throw logic_error( string( log.begin(), log.end() ) ); 
}

GLuint CreateShader( const GLenum aType, const string& aSource )
{
    GLuint shader = glCreateShader( aType );
    const GLchar* shaderString = aSource.c_str();
    glShaderSource( shader, 1, &shaderString, NULL );
    glCompileShader( shader );
    CheckStatus( shader );
    return shader;
}

GLuint prog = 0;
GLuint vbo = 0;
void init()
{
    GLenum glewError = glewInit();
    if( GLEW_OK != glewError )
        throw runtime_error( (char*)glewGetErrorString(glewError) );

    cout << "GL_VERSION   : " << glGetString(GL_VERSION) << endl;
    cout << "GL_VENDOR    : " << glGetString(GL_VENDOR) << endl;
    cout << "GL_RENDERER  : " << glGetString(GL_RENDERER) << endl;    
    cout << "GLEW_VERSION : " << glewGetString(GLEW_VERSION) << endl;
    cout << "GLSL VERSION : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // load shaders
    prog = InitShader( "../vsource.glsl", "../fsource.glsl" );

    //Inicializamos array global
    divide_triangle
        (
        vec2(0.0,0.0),
        vec2(1.0,0.0),
        vec2(0.5,1.0),
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

    glUseProgram( prog );

    GLint position_loc = glGetAttribLocation( prog, "position" );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glVertexAttribPointer( position_loc, 2, GL_FLOAT, GL_FALSE, sizeof( vec2 ), 0 );
    glEnableVertexAttribArray( position_loc );

    glDrawArrays( GL_POINTS, 0, Global::points.size() );

    glDisableVertexAttribArray( position_loc );

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit( &argc, argv );
    glutInitWindowSize( 800,600 );
   glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
   // glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA);
    glutCreateWindow( "Sierpinski gasket" );
    init();
    glutDisplayFunc( display );
    glutMainLoop();
    return 0;
}