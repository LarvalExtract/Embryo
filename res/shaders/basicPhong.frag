#version 330 core

#define MAX_LIGHT_SOURCES 8

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 cameraVector;

out vec4 out_colour;

uniform sampler2D diffuseTexture;

uniform int u_numLights;
uniform vec3 u_lightColour[MAX_LIGHT_SOURCES];
uniform float u_lightPower[MAX_LIGHT_SOURCES];
in vec3 u_surfaceToLight[MAX_LIGHT_SOURCES];

uniform float specularity;
uniform float glossiness;
uniform float ambience;

void main()
{ 
	vec3 unitSurfaceNormal = normalize(surfaceNormal);
	vec3 unitCameraVector = normalize(cameraVector);

	vec4 totalDiffuse = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 totalSpecular = vec4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < u_numLights; i++)
	{
		vec3 unitSurfaceToLight = normalize(u_surfaceToLight[i]);

		vec3 reflectedLightDirection = reflect(-unitSurfaceToLight, unitSurfaceNormal);

		float specFactor = max(dot(reflectedLightDirection, unitCameraVector), 0.0);
		float damped = pow(specFactor, glossiness);

		totalDiffuse += max(u_lightPower[i] * vec4(u_lightColour[i] * max(dot(unitSurfaceNormal, unitSurfaceToLight), 0.0), 1.0), 0.0);
		totalSpecular += max(u_lightPower[i] * vec4(u_lightColour[i] * damped * specularity, 1.0), 0.0);
	}

	out_colour = max(totalDiffuse, ambience) * texture(diffuseTexture, texCoord) + max(totalSpecular, 0.0);	
}