#include "Renderable3D.h"

//// Base class methods
// Base constructor
// This constructor is called by child classes to initialise inherited variables
Renderable3D::Renderable3D(unsigned int renderMode) :
	position(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	origin(0, 0, 0),
	translationMatrix(Mat4(1.0f)),
	rotationMatrix(Mat4(1.0f)),
	scaleMatrix(Mat4(1.0f)),
	originMatrix(Mat4(1.0f)),
	renderMode(renderMode)
{
	//shader = new Shader("test");						// Change this so that default shader/texture reuse the same pointer for each Renderable3D object,
	//texture = new Texture2D("./resources/bitmaps/missingtexture2.tga");		// rather than creating a new shader/texture
}

Renderable3D::~Renderable3D()
{
	//if (texture != nullptr)
	//	delete texture;
	//
	//if (shader != nullptr)
	//	delete shader;
}

void Renderable3D::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	translationMatrix.Translate(position);
}

void Renderable3D::SetRotation(float x, float y, float z)
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

void Renderable3D::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	scaleMatrix.Scale(scale);
}

void Renderable3D::SetOrigin(float x, float y, float z)
{
	origin.x = x;
	origin.y = y;
	origin.z = z;

	originMatrix.elements[3][0] = x;
	originMatrix.elements[3][1] = y;
	originMatrix.elements[3][2] = z;
}

void Renderable3D::SetShader(Shader *pShader)
{
	shader = pShader;
}

void Renderable3D::SetRenderMode(unsigned int mode)
{
	renderMode = mode;
}

void Renderable3D::SetName(const std::string& name)
{
	this->name = name;
}

std::string Renderable3D::GetName()
{
	return name;
}

Shader& Renderable3D::GetShader()
{
	return *shader;
}

void Renderable3D::Draw()
{
	vao.Bind();
	shader->Bind();
	vao.DrawElements(renderMode);
}

void Renderable3D::Draw(Camera &camera, Mat4 &vpMatrix)
{
	// Update shader matrices
	GetShader().Bind();
	GetShader().SetUniformMat4("transformMatrix", GetModelMatrix());
	GetShader().SetUniformMat4("viewMatrix", camera.GetViewMatrix());
	GetShader().SetUniformMat4("mvpMatrix", GetModelMatrix() * vpMatrix);

	Draw();
}