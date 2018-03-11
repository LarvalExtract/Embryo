#version 330 core

layout(location = 0) in vec2 in_position;

uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * vec4(in_position, 0.0, 1.0);
}