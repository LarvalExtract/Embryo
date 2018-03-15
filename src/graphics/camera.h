#pragma once

#include <maths/vec3.h>
#include <maths/mat4.h>

// Base camera
class Camera
{
public:
	virtual ~Camera();

	virtual void SetPosition(float x, float y, float z);

	Vec3<float> position;
	Vec3<float> forwardVector;
	Vec3<float> upwardVector;
	Mat4 projectionMatrix;
	Mat4 viewMatrix;

protected:
	Camera();

	float zNear;
	float zFar;
};