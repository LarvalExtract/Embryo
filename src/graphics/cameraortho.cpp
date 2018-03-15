#include "cameraortho.h"

CamOrtho::CamOrtho(float xLeft, float xRight, float yBottom, float yTop, float zNear, float zFar) :
	xLeft(xLeft), xRight(xRight), yBottom(yBottom), yTop(yTop)
{
	projectionMatrix.Orthographic(xLeft, xRight, yBottom, yTop, zNear, zFar);
	viewMatrix.LookAt(position, position + forwardVector, upwardVector);
}

CamOrtho::~CamOrtho()
{
}