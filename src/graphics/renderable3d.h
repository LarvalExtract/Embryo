#pragma once

#include <string>
#include <vector>

#include <maths/vec3.h>
#include <maths/mat4.h>

#include "vertexarray.h"
#include "shader.h"
#include "texture.h"

#include "camera.h"

// Base class for Renderable3D objects (excluding skybox)
class Renderable3D
{
public:
	Renderable3D(unsigned int renderMode);
	virtual ~Renderable3D();

	virtual inline Mat4 GetModelMatrix() const
	{
		return scaleMatrix * originMatrix * rotationMatrix * translationMatrix;
	}

	std::string GetName();
	Shader& GetShader();

	void SetName(const std::string &name);
	void SetRenderMode(unsigned int mode);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetOrigin(float x, float y, float z);
	void SetShader(Shader *pShader);

	virtual void Draw();
	virtual void Draw(Camera &camera, Mat4 &vpMatrix);

protected:
	std::string name;
	unsigned int renderMode;

	Vec3<float> position;
	Vec3<float> rotation;
	Vec3<float> scale;
	Vec3<float> origin;

	Mat4 translationMatrix;
	Mat4 rotationMatrix;
	Mat4 scaleMatrix;
	Mat4 originMatrix;

	VertexArray  vao;
	Shader		 *shader;
};