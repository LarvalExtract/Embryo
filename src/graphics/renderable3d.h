#pragma once

#include <string>
#include <vector>

#include <maths/Vec3.h>
#include <maths/mat4.h>

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "camera.h"

// Base class for Renderable3D objects (excluding skybox)
class Renderable3D
{
public:
	Renderable3D(unsigned int renderMode);
	virtual ~Renderable3D();

	virtual inline mat4 GetModelMatrix() const
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
	virtual void Draw(Camera &camera, mat4 &vpMatrix);

protected:
	Vec3<float> position;
	Vec3<float> rotation;
	Vec3<float> scale;

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


	


// Pivot
