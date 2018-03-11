#version 330 core

#define MAX_LIGHT_SOURCES 8

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

//out vec3 normal;
out vec2 texCoord;
out vec3 surfaceNormal;
out vec3 lightVector;
out vec3 cameraVector;

uniform mat4 transformMatrix;
uniform mat4 viewMatrix;
uniform mat4 mvpMatrix;

uniform vec3 lightPosition;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(in_position, 1.0);

	surfaceNormal = (transformMatrix * vec4(in_normal, 0.0)).xyz;
	lightVector = lightPosition - worldPosition.xyz;
	cameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	texCoord = in_texCoord;

	gl_Position = mvpMatrix * vec4(in_position, 1.0);
	
}