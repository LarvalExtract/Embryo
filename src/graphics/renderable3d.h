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
		return scaleMatrix * rotationMatrix * translationMatrix;
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

	void SetShader(Shader *pShader);
	void SetRenderMode(unsigned int mode);

	void SetSpecularity(float value);
	void SetGlossiness(float value);

	void SetName(const std::string &name);

	virtual void Draw();
	virtual void Draw(Camera &camera, Mat4 &vpMatrix);

protected:
	Vec3<float> position;
	Vec3<float> rotation;
	Vec3<float> scale;

	Mat4 translationMatrix;
	Mat4 rotationMatrix;
	Mat4 scaleMatrix;

	std::string name;

	unsigned int renderMode;

	VertexArray  vao;
	Texture		 *texture;
	Shader		 *shader;

	float specularity;
	float glossiness;
};