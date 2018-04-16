#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 2) in vec2 in_texCoord;

out vec2 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 transformMatrix;
uniform mat4 mpMatrix;

void main()
{
	gl_Position = mpMatrix * vec4(in_position, 0.0, 1.0);
	texCoord = in_texCoord;
}