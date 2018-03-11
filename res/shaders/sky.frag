#version 330 core

in vec3 texCoord;

out vec4 colour;

uniform samplerCube sky;

void main()
{
	colour = texture(sky, texCoord);
}