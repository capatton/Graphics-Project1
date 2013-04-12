#version 120
attribute vec2 position;

varying vec4 vColor;

void main()
{
   vColor = gl_Color;
   gl_Position = vec4( position, 0.0, 1.0 );
}