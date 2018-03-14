#include "primitive.h"

Primitive::~Primitive()
{
}

void Primitive::SetColour(float red, float green, float blue, float alpha)
{
	colour.x = red;
	colour.y = green;
	colour.z = blue;
	colour.w = alpha;

	shader->SetUniformVec4f("wire_colour", colour);
}

void Primitive::Draw()
{
	vao.Bind();
	shader->Bind();

	vao.DrawElements(renderMode);
}