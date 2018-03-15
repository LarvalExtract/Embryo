#pragma once

#include <maths/vec3.h>
#include <maths/mat4.h>

class Camera
{
public:
	virtual ~Camera() {}

	virtual void SetPosition(float x, float y, float z) { position.x = x, position.y = y, position.z = z, viewMatrix.LookAt(position, position + forwardVector, upwardVector); }

	Vec3<float> position;
	Vec3<float> forwardVector;
	Vec3<float> upwardVector;
	Mat4 projectionMatrix;
	Mat4 viewMatrix;

protected:
	Camera() : position(0, 0, 0), forwardVector(0, 0, 1), upwardVector(0, 1, 0), projectionMatrix(Mat4(0)), viewMatrix(Mat4(1)) {}
};