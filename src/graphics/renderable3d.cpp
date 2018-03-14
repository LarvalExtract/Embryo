#include "Renderable3D.h"

//// Base class methods
// Base constructor
// This constructor is called by child classes to initialise inherited variables
Renderable3D::Renderable3D(unsigned int renderMode) :
	position(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	translationMatrix(mat4::Translation(position)),
	rotationMatrix(mat4::Rotation(0, rotation)),
	scaleMatrix(mat4::Scale(scale)),
	renderMode(renderMode),
	specularity(0),
	glossiness(1)
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

void Renderable3D::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	translationMatrix = mat4::Translation(position);
}

void Renderable3D::SetRot(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	rotationMatrix = mat4::Rotation(rotation.z, 
		Vec3<float>(0.0f, 0.0f, 1.0f)) * mat4::Rotation(rotation.y, Vec3<float>(0.0f, 1.0f, 0.0f)) * mat4::Rotation(rotation.x, Vec3<float>(1.0f, 0.0f, 0.0f));
}

void Renderable3D::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	scaleMatrix = mat4::Scale(scale);
}

void Renderable3D::SetShader(Shader *pShader)
{
	shader = pShader;
}

void Renderable3D::SetRenderMode(unsigned int mode)
{
	renderMode = mode;
}

void Renderable3D::SetSpecularity(float value)
{
	specularity = value;
}

void Renderable3D::SetGlossiness(float value)
{
	glossiness = value;
}
void Renderable3D::SetName(const std::string& name)
{
	this->name = name;
}

Vec3<float> Renderable3D::GetPos()
{
	return position;
}

Vec3<float> Renderable3D::GetRot()
{
	return rotation;
}

Vec3<float> Renderable3D::GetScale()
{
	return scale;
}

std::string Renderable3D::GetName()
{
	return name;
}

Texture& Renderable3D::GetTexture()
{
	return *texture;
}

Shader& Renderable3D::GetShader()
{
	return *shader;
}

float Renderable3D::GetSpecularity()
{
	return specularity;
}

float Renderable3D::GetGlossiness()
{
	return glossiness;
}

void Renderable3D::Draw()
{
	vao.Bind();
	shader->Bind();
	texture->Bind(0);
	vao.DrawElements(renderMode);
}

void Renderable3D::Draw(Camera &camera, mat4 &vpMatrix)
{
	// Update shader matrices
	GetShader().Bind();
	GetShader().SetUniformMat4("transformMatrix", GetModelMatrix());
	GetShader().SetUniformMat4("viewMatrix", camera.GetViewMatrix());
	GetShader().SetUniformMat4("mvpMatrix", GetModelMatrix() * vpMatrix);

	// Update shader lighting components
	GetShader().SetUniformFloat("specularity", GetSpecularity());
	GetShader().SetUniformFloat("glossiness", GetGlossiness());

	Draw();
}