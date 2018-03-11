#pragma once

#include <./maths/vec3.h>
#include <./maths/mat4.h>

#include <AL/al.h>

class Camera
{
public:
	Camera();
	Camera(const vec3& pos, float fov, float aspectRatio, float zNear, float zFar);

	void Init(const vec3& pos, float fov, float aspectRatio, float zNear, float zFar);

	mat4 GetProjectionMatrix();
	mat4 GetViewMatrix();

	vec3  GetPos();
	vec3  GetForward();
	vec3  GetUpward();
	float GetFOV();
	float GetAspectRatio();

	void SetPosition(const float& x, const float& y, const float& z);
	void SetForward(const float& x, const float& y, const float& z);
	void SetUpward(const float& x, const float& y, const float& z);
	void SetFOV(const float& fov);
	void SetAspectRatio(const float& aspectRatio);

public:
	vec3 position;
	vec3 forward;
	vec3 upward;

	mat4 projectionMatrix;
	mat4 viewMatrix;

	float fov;
	float aspectRatio;
	float clipNear;
	float clipFar;
};