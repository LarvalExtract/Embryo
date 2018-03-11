#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 3) in vec4 in_colour;

out vec4 out_colour;

uniform mat4 transformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(in_position, 1.0);
	out_colour = in_colour;
}