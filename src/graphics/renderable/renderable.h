#pragma once

#include <maths/vec3.h>
#include <maths/vec4.h>
#include <maths/mat4.h>
#include <graphics/shader.h>
#include <graphics/vertexarray.h>
#include <graphics/camera.h>
#include <utilities/logger.h>
#include <string>

// Base class for all renderable objects
class Renderable
{
public:
	virtual ~Renderable();

	inline Mat4 GetTransformMatrix() { return scaleMatrix * localOriginMatrix * rotationMatrix * translationMatrix; }

	virtual void Draw(Camera &camera, Mat4 &cameraMatrix) = 0;
	virtual void CentreLocalOrigin();
	virtual void SetPosition(float x, float y, float z);	
	virtual void SetRotation2D(float z);					// Use this function to rotate objects on the Z axis (should only be used with 2D stuff)
	virtual	void SetRotation3D(float x, float y, float z);	// Use this function to rotate objects on any axis (should only be used with 3D stuff)
	virtual void SetScale(float x, float y, float z);
	virtual void SetLocalOrigin(float x, float y, float z);
	virtual void SetColour(float r, float g, float b, float a);
	virtual void SetRenderMode(unsigned int renderMode);
	virtual void SetShader(Shader *pShader);

	std::string name;
	Vec3<float> position;
	Vec3<float> rotation;	// Is this even needed?
	Vec3<float> scale;
	Vec3<float> localOrigin;
	Vec4<float> colour;

protected:
	Renderable(unsigned int renderMode);

	Mat4 translationMatrix;
	Mat4 rotationMatrix;
	Mat4 scaleMatrix;
	Mat4 localOriginMatrix;
	float width;
	float height;
	float depth;

	unsigned int renderMode;
	VertexArray vao;
	Shader *shader;
	//static Shader* const defaultShader;
};