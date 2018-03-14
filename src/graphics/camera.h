#pragma once

#include <./maths/Vec3.h>
#include <./maths/mat4.h>

class Camera
{
public:
	Camera();
	Camera(const Vec3<float> &pos, float fov, float aspectRatio, float zNear, float zFar);

	void Init(const Vec3<float> &pos, float fov, float aspectRatio, float zNear, float zFar);

	Mat4 GetProjectionMatrix();
	Mat4 GetViewMatrix();

	Vec3<float> GetPos();
	Vec3<float> GetForward();
	Vec3<float> GetUpward();
	float GetFOV();
	float GetAspectRatio();

	void SetPosition(float x, float y, float z);
	void SetForward(float x, float y, float z);
	void SetUpward(float x, float y, float z);
	void SetFOV(float fov);
	void SetAspectRatio(float aspectRatio);

public:
	Vec3<float> position;
	Vec3<float> forward;
	Vec3<float> upward;

	Mat4 projectionMatrix;
	Mat4 viewMatrix;

	float fov;
	float aspectRatio;
	float clipNear;
	float clipFar;
};