#pragma once

#include <maths/Vec3.h>
#include <maths/Vec4.h>
#include <graphics/renderable/sprite.h>
#include "shader.h"

class LightSource
{
public:
	virtual ~LightSource();

	virtual void Attenuate(Shader &shader, const unsigned char index) = 0;

	void SetPosition(float x, float y, float z);
	void SetColour(float r, float g, float b);

	std::string name;
	Vec3<float> position;
	Vec3<float> colour;
	float power;

protected:
	LightSource();
	
};