#pragma once

#include <string>
#include <vector>

#include <maths/vec3.h>
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

	virtual inline mat4 GetModelMatrix() const
	{
		return scaleMatrix * rotationMatrix * translationMatrix;
	}

	Texture& GetTexture();
	Shader& GetShader();

	float GetSpecularity();
	float GetGlossiness();

	vec3 GetPos();
	vec3 GetRot();
	vec3 GetScale();

	std::string GetName();

	void SetPos(const float& x, const float& y, const float& z);
	void SetRot(const float& x, const float& y, const float& z);
	void SetScale(const float& x, const float& y, const float& z);

	void SetShader(Shader *pShader);
	void SetRenderMode(const unsigned int& mode);

	void SetSpecularity(const float& value);
	void SetGlossiness(const float& value);

	void SetName(const std::string& name);

	virtual void Draw();

protected:
	// Base constructor
	// This constructor is called by child classes to initialise inherited variables
	Renderable3D(const unsigned int& renderMode);

	vec3 position;
	vec3 rotation;
	vec3 scale;

	mat4 translationMatrix;
	mat4 rotationMatrix;
	mat4 scaleMatrix;

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

	void SetTexture(const std::string& filePath);
	void SetNormalMap(const std::string& filePath);
	void SetSpecularMap(const std::string& filePath);
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
	inline mat4 GetModelMatrix() const override 
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
		void SetColour(const float& red, const float& green, const float& blue, const float& alpha);

		void Draw() override;
	protected:
		Primitive(const unsigned int& renderMode, const vec4& colour) : 
			Renderable3D(renderMode), colour(colour) {};

		vec4 colour;
	};

// Box
	class Box : public Primitive
	{
	public:
		Box(const float& width, const float& height, const float& depth, const vec4& colour);

	private:

	};

// Sphere
	class Sphere : public Primitive
	{
	public:
		Sphere(unsigned char segments, const float& radius, const vec4& colour);

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