#include "LightSource.h"

LightSource::LightSource(const vec3& position, const vec3& colour, const float& brightness) :
	position(position),
	colour(colour),
	power(brightness)
{
}

vec3 LightSource::GetPosition()
{
	return position;
}

vec3 LightSource::GetColour()
{
	return colour;
}

float LightSource::GetPower()
{
	return power;
}

std::string LightSource::GetName()
{
	return name;
}

void LightSource::SetPosition(const float& x, const float& y, const float& z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void LightSource::SetColour(const float& r, const float& g, const float& b)
{
	colour.x = r;
	colour.y = g;
	colour.z = b;
}

void LightSource::SetPower(const float& value)
{
	power = value;
}

void LightSource::SetName(const std::string& name)
{
	this->name = name;
}

LightOmni::LightOmni(const vec3& position, const vec3& colour, const float& brightness) :
	LightSource(position, colour, brightness)
{
	name = "light_omni";
}