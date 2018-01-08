#version 330 core

//https://en.wikibooks.org/wiki/OpenGL_Programming/Post-Processing

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;
uniform float offSet;

void main()
{
	vec2 texCoord=textureCoordsOut;
	texCoord.x+=sin(texCoord.y*4*2*3.14159+offSet)/100;
	colour=texture2D(texture0,texCoord);
}