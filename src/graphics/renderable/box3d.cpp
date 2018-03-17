#include "box3d.h"

Box3D::Box3D(float width, float height, float depth) :
	Renderable(GL_LINES)
{
	name = "Box";

	this->width = width;
	this->height = height;
	this->depth = depth;

	float positions[] = {
		//   X		 Y		 Z
		-width,  height, -depth,	// 0
		 width,  height, -depth,	// 1
		 width,  height,  depth,	// 2
		-width,  height,  depth,	// 3	 													 	
		-width, -height, -depth,	// 4
		 width, -height, -depth,	// 5
		 width, -height,  depth,	// 6
		-width, -height,  depth		// 7
	};

	unsigned short indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,		// Top
		0, 4, 1, 5, 2, 6, 3, 7,		// Sides
		4, 5, 5, 6, 6, 7, 7, 4		// Bottom
	};

	vao.AddBuffer(positions, sizeof(positions), 0, 3);
	vao.AddIndices(indices, 24);

	shader = new Shader("basiccolour");
	shader->Bind();
	shader->SetUniformVec4f("u_colour", colour);
}

void Box3D::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	vao.Bind();
	shader->Bind();
	shader->SetUniformMat4("transformMatrix", GetTransformMatrix());
	shader->SetUniformMat4("viewMatrix", camera.viewMatrix);
	shader->SetUniformMat4("mvpMatrix", GetTransformMatrix() * cameraMatrix);
	vao.DrawElements(renderMode);
}