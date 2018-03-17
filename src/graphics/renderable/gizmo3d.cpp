#include "gizmo3d.h"

Gizmo3D::Gizmo3D() :
	Renderable(GL_LINES)
{
	name = "Gizmo";

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

void Gizmo3D::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	vao.Bind();
	shader->Bind();
	shader->SetUniformMat4("mvpMatrix", GetTransformMatrix() * cameraMatrix);
	vao.DrawArrays(renderMode);
}