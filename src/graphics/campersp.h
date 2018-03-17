#pragma once

#include "camera.h"

// Camera with perspective rendering
class CamPersp : public Camera
{
public:
	CamPersp(float fov, float aspectRatio, float zNear, float zFar);
	virtual ~CamPersp();

	void SetFOV(float value);
	void SetAspectRatio(float value);

private:
	float fov;
	float aspectRatio;
};