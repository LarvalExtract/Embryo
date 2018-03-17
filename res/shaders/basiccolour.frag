#version 330 core

out vec4 out_colour;

uniform vec4 u_colour;

void main()
{
	out_colour = u_colour;
}