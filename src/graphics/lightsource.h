#pragma once

#include <maths/Vec3.h>
#include <maths/Vec4.h>
#include <graphics/renderable/sprite.h>

class LightSource
{
public:
	virtual ~LightSource();

	std::string name;
	Vec3<float> position;
	Vec3<float> colour;
	float power;

protected:
	LightSource(const Vec3<float> &position, const Vec3<float> &colour, float brightness);
	
};