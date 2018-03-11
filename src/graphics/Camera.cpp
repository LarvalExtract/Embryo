#include "camera.h"

Camera::Camera() :
	position(0, 1, -3),
	forward(0, 0, 1),
	upward(0, 1, 0)
{
}

Camera::Camera(const vec3& pos, float fov, float aspectRatio, float zNear, float zFar) :
	position(pos),
	forward(0, 0, 1),
	upward(0, 1, 0),
	fov(fov),
	aspectRatio(aspectRatio),
	clipNear(zNear),
	clipFar(zFar)
{
	projectionMatrix = mat4::Perspective(fov, aspectRatio, zNear, zFar);
	viewMatrix = mat4::LookAt(position, position + forward, upward);
}

void Camera::Init(const vec3& pos, float fov, float aspectRatio, float zNear, float zFar)
{
	projectionMatrix = mat4::Perspective(fov, aspectRatio, zNear, zFar);
	viewMatrix = mat4::LookAt(position, position + forward, upward);
}

mat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

mat4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

vec3 Camera::GetPos()
{
	return position;
}

vec3 Camera::GetForward()
{
	return forward;
}

vec3 Camera::GetUpward()
{
	return upward;
}

float Camera::GetFOV()
{
	return fov;
}

float Camera::GetAspectRatio()
{
	return aspectRatio;
}

void Camera::SetPosition(const float& x, const float& y, const float& z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::SetForward(const float& x, const float& y, const float& z)
{
	forward.x = x;
	forward.y = y;
	forward.z = z;
}

void Camera::SetUpward(const float& x, const float& y, const float& z)
{
	upward.x = x;
	upward.y = y;
	upward.z = z;
}

void Camera::SetFOV(const float& fov)
{
	this->fov = fov;
	projectionMatrix = mat4::Perspective(fov, aspectRatio, clipNear, clipFar);
}

void Camera::SetAspectRatio(const float& aspectRatio)
{
	this->aspectRatio = aspectRatio;
	projectionMatrix = mat4::Perspective(fov, aspectRatio, clipNear, clipFar);
}