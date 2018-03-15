#pragma once

#include "camera.h"

class CamOrtho : public Camera
{
public:
	CamOrtho(float xLeft, float xRight, float yBottom, float yTop, float zNear, float zFar);

private:
	float xLeft;
	float xRight;
	float yBottom;
	float yTop;
	float zNear;
	float zFar;
};