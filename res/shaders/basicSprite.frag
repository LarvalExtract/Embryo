#version 330 core

in vec2 texCoord;

out vec4 colour;

uniform sampler2D sprite;

void main()
{
	colour = texture(sprite, texCoord);
}
