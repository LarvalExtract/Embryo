#version 330 core

in vec2 texCoord;

out vec4 colour;

uniform sampler2D textSprite;

void main()
{
	colour = vec4(1.0, 1.0, 1.0, texture(textSprite, texCoord).r);
}