#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

out vec2 texCoord;

uniform mat4 u_transformMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_mvpMatrix;

void main()
{
    texCoord = in_texCoord;
    gl_Position = u_mvpMatrix * vec4(in_position, 1.0);
}