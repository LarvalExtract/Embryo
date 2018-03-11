#version 330 core

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 lightVector;
in vec3 cameraVector;

out vec4 out_colour;

uniform sampler2D diffuseTexture;

uniform vec3 lightColour;
uniform float lightPower;

uniform float specularity;
uniform float glossiness;
uniform float ambience;

void main()
{ 
	vec3 normalizedSurfaceNormal = normalize(surfaceNormal);
	vec3 normalizedLightVector = normalize(lightVector);
	vec3 normalizedCameraVector = normalize(cameraVector);

	vec3 reflectedLightDirection = reflect(-normalizedLightVector, normalizedSurfaceNormal);

	float specFactor = max(dot(reflectedLightDirection, normalizedCameraVector), 0.0);
	float damped = pow(specFactor, glossiness);

	vec4 diffuse = vec4(lightColour * max(dot(normalizedSurfaceNormal, normalizedLightVector), 0.0), 1.0);
	vec4 specular = vec4(lightColour * damped * specularity, 1.0);

	out_colour =  max(lightPower * diffuse, ambience) * texture(diffuseTexture, texCoord) + max(lightPower * specular, 0.0);
	
}