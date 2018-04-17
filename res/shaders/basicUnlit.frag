#version 330 core

in vec2 texCoord;

out vec4 colour;

uniform sampler2D diffuseTexture;

void main()
{
    colour = texture(diffuseTexture, texCoord);
}