#pragma once

#include <string>
#include <vector>

#include <maths/Vec3.h>
#include <maths/mat4.h>

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// Base class for Renderable3D objects (excluding skybox)
class Renderable3D
{
public:
	// Base destructor
	virtual ~Renderable3D();

	virtual inline Matrix4x4 GetModelMatrix()
	{
		return scaleMatrix * originMatrix * rotationMatrix * translationMatrix;
	}

	Texture& GetTexture();
	Shader& GetShader();

	float GetSpecularity();
	float GetGlossiness();

	Vec3<float> GetPos();
	Vec3<float> GetRot();
	Vec3<float> GetScale();

	std::string GetName();

	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetOrigin(float x, float y, float z);

	void SetShader(Shader *pShader);
	void SetRenderMode(unsigned int mode);

	void SetSpecularity(float value);
	void SetGlossiness(float value);

	void SetName(const std::string &name);

	virtual void Draw();

protected:
	// Base constructor
	// This constructor is called by child classes to initialise inherited variables
	Renderable3D(unsigned int renderMode);

	Vec3<float> position;
	Vec3<float> rotation;
	Vec3<float> scale;
	Vec3<float> origin;

	Matrix4x4 translationMatrix;
	Matrix4x4 rotationMatrix;
	Matrix4x4 scaleMatrix;
	Matrix4x4 originMatrix;

	std::string name;

	unsigned int renderMode;

	VertexArray  vao;
	Texture		 *texture;
	Shader		 *shader;

	float specularity;
	float glossiness;
};

// Render 3D models from loaded .mmf files
// Inherits from Renderable3D super-class
class Model3D : public Renderable3D
{
public:
	Model3D(const std::string& filePath);

	void SetTexture(const std::string &filePath);
	void SetNormalMap(const std::string &filePath);
	void SetSpecularMap(const std::string &filePath);
private:

};

// Render 2D sprites from textures
// Inherits from Renderable3D super-class
class Sprite2D : public Renderable3D
{
public:
	Sprite2D(const std::string& filePath);

	void Draw() override;

private:

};

// Renders a skybox
class Skybox : public Renderable3D
{
public:
	Skybox(const std::string& skyboxName);

	// Overrides base class GetModelMatrix() function
	// The skybox must not calculate position or scale matrices, therefore it must have its own version
	inline Matrix4x4 GetModelMatrix() override 
	{
		return rotationMatrix;
	};

private:
	float scale;
};


// Namespace for all primitive objects
// These are useful for debugging purposes, and are expected to be drawn as wireframe objects
namespace Primitive
{
	// Base class for primitive shapes
	// Inherits from Renderable3D
	class Primitive : public Renderable3D
	{
	public:
		void SetColour(float red, float green, float blue, float alpha);

		void Draw() override;
	protected:
		Primitive(unsigned int renderMode, const Vec4<float> &colour) : 
			Renderable3D(renderMode), colour(colour) {};

		Vec4<float> colour;
	};

// Box
	class Box : public Primitive
	{
	public:
		Box(float width, float height, float depth, const Vec4<float> & colour);

	private:

	};

// Sphere
	class Sphere : public Primitive
	{
	public:
		Sphere(unsigned char segments, float radius, const Vec4<float> &colour);

	private:

	};

// Pivot
	class Pivot : public Primitive
	{
	public:
		Pivot();

		void Draw() override;

	private:

	};
}