#pragma once

#include "camera.h"

// Camera with orthographic rendering
class CamOrtho : public Camera
{
public:
	CamOrtho(float xLeft, float xRight, float yBottom, float yTop, float zNear, float zFar);
	virtual ~CamOrtho();

private:
	float xLeft;
	float xRight;
	float yBottom;
	float yTop;
};