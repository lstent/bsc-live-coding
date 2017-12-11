#version 330 core

layout(location=0) in vec3 vertexPosition;
layout(location=1) in vec4 vertexColour;
layout(location=2) in vec2 vertexTextureCoord;
layout(location=3) in vec3 vertexNormals;

uniform float time=0.0f;

uniform mat4 modelMatrix=mat4(1.0f);
uniform mat4 viewMatrix=mat4(1.0f);
uniform mat4 projectionMatrix=mat4(1.0f);

//lighting
uniform vec3 lightDirection;
uniform vec4 diffuseLightColour;

//Material
uniform vec4 diffuseMaterialColour;

out vec4 vertexColourOut;
out vec2 vertexTextureCoordOut;
out vec4 diffuse;

void main()
{
	mat4 MVPMatrix=projectionMatrix*viewMatrix*modelMatrix;

	//calculate lighting
	float nDotl=dot(vertexNormals,lightDirection);
	diffuse=diffuseMaterialColour*diffuseLightColour*nDotl;

	gl_Position=MVPMatrix*vec4(vertexPosition,1.0f);
	vertexColourOut=vertexColour;
	vertexTextureCoordOut=vertexTextureCoord;
}