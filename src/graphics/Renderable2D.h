#pragma once

#include <string>

#include <maths/vec3.h>
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
		return scaleMatrix * rotateMatrix * translateMatrix;
	};

	vec2 GetPosition();
	vec2 GetRotation();
	vec2 GetScale();

	Shader& GetShader();

	void SetPosition(const float& x, const float& y);
	void SetRotation(const float& z);
	void SetScale(const float& x, const float& y);
	void SetOrigin(const float& x, const float& y);

	void SetShader();

	virtual void Draw();

protected:
	Renderable2D(const unsigned int& renderMode);

	VertexArray vao;

	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 origin;

	mat4 translateMatrix;
	mat4 rotateMatrix;
	mat4 scaleMatrix;

	Shader* shader;

	unsigned int renderMode;
};

class Sprite : public Renderable2D
{
public:
	Sprite(const std::string& texturePath);

	void SetTexture(const std::string& texturePath);

	void CenterOrigin();

	void Draw() override;
private:
	Texture2D* texture;

	float width, height;
};



namespace Shape2D
{
	class Shape2D : public Renderable2D
	{
	public:
		void SetColour(const float& r, const float& g, const float& b, const float& a);

	protected:
		Shape2D(const unsigned int& renderMode);

		vec4 colour;
	};

	class Quad : public Shape2D
	{
	public:
		Quad(const float& width, const float& height);

	private:
		float width, height;
	};

	class Triangle : public Shape2D
	{
	public:
		Triangle(const float& size);

		void Draw() override;
	};
}