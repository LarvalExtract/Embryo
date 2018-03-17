#include "renderable.h"

//Shader* const Renderable::defaultShader = new Shader("default");

Renderable::Renderable(unsigned int renderMode) :
	position(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	localOrigin(0, 0, 0),
	colour(1, 1, 1, 1),
	translationMatrix(Mat4(1.0f)),
	rotationMatrix(Mat4(1.0f)),
	scaleMatrix(Mat4(1.0f)),
	localOriginMatrix(Mat4(1.0f)),
	width(0),
	height(0),
	depth(0),
	renderMode(renderMode),
	shader(nullptr)
{
}

Renderable::~Renderable()
{
}

//void Renderable::Draw(Camera &camera)
//{
//	vao.Bind();
//	shader->Bind();
//	shader->SetUniformMat4("transformMatrix", GetTransformMatrix());
//	shader->SetUniformMat4("viewMatrix", camera.viewMatrix);
//	shader->SetUniformMat4("mvpMatrix", GetTransformMatrix() * camera.viewMatrix * camera.projectionMatrix);
//	vao.DrawElements(renderMode);
//}

void Renderable::CentreLocalOrigin()
{
	localOrigin.x = -width / 2.0f;
	localOrigin.y = -height / 2.0f;
	localOrigin.z = -depth / 2.0f;
	localOriginMatrix.Translate(localOrigin);
}

void Renderable::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	translationMatrix.Translate(position);
}

void Renderable::SetRotation2D(float z)
{
	rotationMatrix.Rotate(z, Vec3<float>(0.0f, 0.0f, 1.0f));
}

void Renderable::SetRotation3D(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	Mat4 rotX(1.0f);
	rotX.Rotate(x, Vec3<float>(1.0f, 0.0f, 0.0f));
	Mat4 rotY(1.0f);
	rotY.Rotate(y, Vec3<float>(0.0f, 1.0f, 0.0f));
	Mat4 rotZ(1.0f);
	rotZ.Rotate(z, Vec3<float>(0.0f, 0.0f, 1.0f));

	rotationMatrix = rotZ * rotY * rotX;
}

void Renderable::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	scaleMatrix.Scale(scale);
}

void Renderable::SetLocalOrigin(float x, float y, float z)
{
	localOrigin.x = x;
	localOrigin.y = y;
	localOrigin.z = z;
	localOriginMatrix.Translate(localOrigin);
}

void Renderable::SetColour(float r, float g, float b, float a)
{
	colour.x = r;
	colour.y = g;
	colour.z = b;
	colour.w = a;
	shader->SetUniformVec4f("u_colour", colour);
}

void Renderable::SetRenderMode(unsigned int renderMode)
{
	this->renderMode = renderMode;
}

void Renderable::SetShader(Shader *pShader)
{
	shader = pShader;
}