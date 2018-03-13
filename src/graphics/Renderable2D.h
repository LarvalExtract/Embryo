#pragma once

#include <string>

#include <maths/Vec3.h>
#include <maths/mat4.h>

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

class Renderable2D
{
public:
	virtual ~Renderable2D();

	// S * R * T for global, T * R * S for local
	inline mat4 GetTransformMatrix()
	{
		return scaleMatrix * originMatrix * rotateMatrix * translateMatrix;
	};

	Vec2<float> GetPosition();
	Vec2<float> GetRotation();
	Vec2<float> GetScale();

	Shader& GetShader();

	void SetPosition(float x, float y);
	void SetRotation(float z);
	void SetScale(float x, float y);
	void SetOrigin(float x, float y);

	void SetShader();

	virtual void Draw();

protected:
	Renderable2D(unsigned int renderMode);

	VertexArray vao;

	Vec3<float> position;
	Vec3<float> rotation;
	Vec3<float> scale;
	Vec3<float> origin;

	mat4 translateMatrix;
	mat4 rotateMatrix;
	mat4 scaleMatrix;
	mat4 originMatrix;

	Shader *shader;

	unsigned int renderMode;
};

class Sprite : public Renderable2D
{
public:
	Sprite(const std::string &texturePath);

	void SetTexture(const std::string &texturePath);

	void CenterOrigin();

	void Draw() override;
private:
	Texture2D *texture;

	float width;
	float height;
};

namespace Shape2D
{
	class Shape2D : public Renderable2D
	{
	public:
		void SetColour(float r, float g, float b, float a);

	protected:
		Shape2D(unsigned int renderMode);

		Vec4<float> colour;
	};

	class Quad : public Shape2D
	{
	public:
		Quad(float width, float height);

	private:
		float width, height;
	};

	class Triangle : public Shape2D
	{
	public:
		Triangle(float size);

		void Draw() override;
	};
}