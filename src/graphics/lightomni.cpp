#include "lightomni.h"

LightOmni::LightOmni(const Vec3<float> &position, const Vec3<float> &colour, float brightness) :
	LightSource(position, colour, brightness)
{
	name = "light_omni";
}