#pragma once

#include "camera.h"

class CamPerspective : public Camera
{
public:
	CamPerspective(float fov, float aspectRatio, float zNear, float zFar);

	void SetFOV(float value);
	void SetAspectRatio(float value);

private:
	float fov;
	float aspectRatio;
	float zNear;
	float zFar;
};