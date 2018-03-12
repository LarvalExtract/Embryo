#include "LightSource.h"

LightSource::LightSource(const Vec3<float>& position, const Vec3<float>& colour, float brightness) :
	position(position),
	colour(colour),
	power(brightness)
{
}

Vec3<float> LightSource::GetPosition()
{
	return position;
}

Vec3<float> LightSource::GetColour()
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

void LightSource::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void LightSource::SetColour(float r, float g, float b)
{
	colour.x = r;
	colour.y = g;
	colour.z = b;
}

void LightSource::SetPower(float value)
{
	power = value;
}

void LightSource::SetName(const std::string& name)
{
	this->name = name;
}

LightOmni::LightOmni(const Vec3<float> &position, const Vec3<float> &colour, float brightness) :
	LightSource(position, colour, brightness)
{
	name = "light_omni";
}