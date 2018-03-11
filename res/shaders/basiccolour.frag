#version 330 core

out vec4 colour;

uniform vec4 wire_colour;

void main()
{
	colour = wire_colour;
}