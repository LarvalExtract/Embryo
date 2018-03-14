#pragma once

#include "Renderable3D.h"

// Base class for 3D renderable shapes
class Primitive : public Renderable3D
{
public:
	Primitive(unsigned int renderMode, const Vec4<float> &colour) :
		Renderable3D(renderMode), colour(colour) {};

	virtual ~Primitive();

	virtual void SetColour(float red, float green, float blue, float alpha);

	virtual void Draw() override;

protected:
	Vec4<float> colour;

};