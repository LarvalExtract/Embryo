#include "model3d.h"

Texture2D* const Model3D::defaultTexture = new Texture2D("missingtexture2.tga");

Model3D::Model3D() :
	Renderable(GL_TRIANGLES),
	specularity(0),
	glossiness(1),
	diffuseMap(defaultTexture),
	normalMap(nullptr),
	specularMap(nullptr)
{
	
}

Model3D::~Model3D()
{
	if (diffuseMap != nullptr)
		if (diffuseMap != defaultTexture)
			delete diffuseMap;

	if (normalMap != nullptr)
		if (normalMap != defaultTexture)
			delete normalMap;

	if (specularMap != nullptr)
		if (specularMap != defaultTexture)
			delete specularMap;
}

void Model3D::SetDiffuseTexture(const std::string &fileName)
{
	if (diffuseMap != nullptr)
		if (diffuseMap != defaultTexture)
			delete diffuseMap;

	diffuseMap = new Texture2D(fileName);
}

void Model3D::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	vao.Bind();
	shader->Bind();

	// Update shader matrices
	shader->SetUniformMat4("transformMatrix", GetTransformMatrix());
	shader->SetUniformMat4("viewMatrix", camera.viewMatrix);
	shader->SetUniformMat4("mvpMatrix", GetTransformMatrix() * cameraMatrix);

	// Update shader specular and gloss values
	shader->SetUniformFloat("specularity", specularity);
	shader->SetUniformFloat("glossiness", glossiness);

	diffuseMap->Bind(0);

	vao.DrawElements(renderMode);
}