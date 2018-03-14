#pragma once

#include "lightsource.h"

class LightOmni : public LightSource
{
public:
	LightOmni(const Vec3<float> &position, const Vec3<float> &colour, float brightness);

private:

};