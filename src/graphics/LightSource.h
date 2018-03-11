#pragma once

#include <maths/vec3.h>
#include <maths/vec4.h>

class LightSource
{
public:
	vec3 GetPosition();
	vec3 GetColour();
	float GetPower();

	std::string GetName();

	void SetPosition(const float& x, const float& y, const float& z);
	void SetColour(const float& r, const float& g, const float& b);
	void SetPower(const float& value);

	void SetName(const std::string& name);

protected:
	LightSource(const vec3& position, const vec3& colour, const float& brightness);

	vec3 position;
	vec3 colour;

	std::string name;

	float power;
};

class LightOmni : public LightSource
{
public:
	LightOmni(const vec3& position, const vec3& colour, const float& brightness);

private:

};