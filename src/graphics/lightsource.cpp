#include "LightSource.h"

LightSource::LightSource() :
	position(0, 0, 0),
	colour(1, 1, 1),
	power(1)
{
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

LightSource::~LightSource()
{
}