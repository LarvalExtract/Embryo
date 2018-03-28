#version 330 core

// Maximum amount of light sources the shader will compute
#define MAX_LIGHT_SOURCES 8

// Layout atrribute locations
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

//out vec3 normal;
out vec2 texCoord;
out vec3 surfaceNormal;
out vec3 cameraVector;

// Uniform matrices
// u_mvpMatrix is calculated by the CPU, not the GPU
uniform mat4 u_transformMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_mvpMatrix;

//struct LightOmni
//{
//	vec3 position;
//	vec4 colour;
//	float power;
//	vec3 surfaceToLight;
//}

// Light data
uniform int u_numLights;
uniform vec3 u_lightPosition[MAX_LIGHT_SOURCES];
out vec3 surfaceToLight[MAX_LIGHT_SOURCES];

void main()
{
	vec4 worldPosition = u_transformMatrix * vec4(in_position, 1.0);
	cameraVector = (inverse(u_viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
	surfaceNormal = (u_transformMatrix * vec4(in_normal, 0.0)).xyz;

	for (int i = 0; i < u_numLights; i++)
	{
		surfaceToLight[i] = u_lightPosition[i] - worldPosition.xyz;
	}

	texCoord = in_texCoord;

	gl_Position = u_mvpMatrix * vec4(in_position, 1.0);
}