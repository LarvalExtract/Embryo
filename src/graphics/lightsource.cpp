#include "LightSource.h"

LightSource::LightSource(const Vec3<float>& position, const Vec3<float>& colour, float brightness) :
	position(position),
	colour(colour),
	power(brightness)
{
}

LightSource::~LightSource()
{
}