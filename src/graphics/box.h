#pragma once

#include "primitive.h"

// 3D renderable wireframe box
class Box : public Primitive
{
public:
	Box(float width, float height, float depth, const Vec4<float> & colour);

	virtual void Draw() override;
};