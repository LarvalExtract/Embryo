#pragma once

#include "primitive.h"

// 3D renderable wireframe sphere
class Sphere : public Primitive
{
public:
	Sphere(unsigned char segments, float radius, const Vec4<float> &colour);

	virtual void Draw() override;
private:

};