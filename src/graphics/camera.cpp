#include "camera.h"

Camera::Camera() :
	position(0, 0, 0),
	forwardVector(0, 0, 1),
	upwardVector(0, 1, 0),
	projectionMatrix(Mat4(0)),
	viewMatrix(Mat4(1))
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	viewMatrix.LookAt(position, position + forwardVector, upwardVector);
}