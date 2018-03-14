#include "pivot.h"

// Pivot sub-class methods
// Inherits from Primitive base class
Pivot::Pivot() :
	Primitive(GL_LINES, Vec4<float>(1, 1, 1, 1))
{
	name = "Pivot";

	float positions[] = {
		0.0f, 0.0f, 0.0f,			// Origin
		1.0f, 0.0f, 0.0f,			// X

		0.0f, 0.0f, 0.0f,			// Origin
		0.0f, 1.0f, 0.0f,			// Y

		0.0f, 0.0f, 0.0f,			// Origin
		0.0f, 0.0f, -1.0f			// Z
	};

	float colours[] = {
		1.0f, 0.0f, 0.0f, 1.0f,		// Red
		1.0f, 0.0f, 0.0f, 1.0f,		// Red
		0.0f, 1.0f, 0.0f, 1.0f,		// Green
		0.0f, 1.0f, 0.0f, 1.0f,		// Green
		0.0f, 0.0f, 1.0f, 1.0f,		// Blue
		0.0f, 0.0f, 1.0f, 1.0f		// Blue
	};

	vao.AddBuffers(positions, sizeof(positions), 0, 3);
	vao.AddBuffers(colours, sizeof(colours), 3, 4);

	shader = new Shader("in_colour");
}

void Pivot::Draw()
{
	Primitive::Draw();
}
