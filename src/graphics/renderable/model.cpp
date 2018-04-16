#include "Model.h"

Model::Model() :
	Renderable(GL_TRIANGLES)
{
	
}

Model::~Model()
{
	
}

void Model::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	vao.Bind();
	shader->Bind();

	// Update shader matrices
	shader->SetUniformMat4("u_transformMatrix", GetTransformMatrix());
	shader->SetUniformMat4("u_viewMatrix", camera.viewMatrix);
	shader->SetUniformMat4("u_mvpMatrix", GetTransformMatrix() * cameraMatrix);

	// Update shader specular and gloss values
	shader->SetUniformFloat("specularity", material.specularity);
	shader->SetUniformFloat("glossiness", material.glossiness);

	material.diffuseMap->Bind(0);

	vao.DrawElements(renderMode);
}