#include "campersp.h"

CamPersp::CamPersp(float fov, float aspectRatio, float zNear, float zFar) :
	fov(fov), aspectRatio(aspectRatio)
{
	projectionMatrix.Perspective(fov, aspectRatio, zNear, zFar);
	viewMatrix.LookAt(position, position + forwardVector, upwardVector);
}

CamPersp::~CamPersp()
{
}

void CamPersp::SetFOV(float value)
{
	fov = value;
	projectionMatrix.Perspective(fov, aspectRatio, zNear, zFar);
}

void CamPersp::SetAspectRatio(float value)
{
	aspectRatio = value;
	projectionMatrix.Perspective(fov, aspectRatio, zNear, zFar);
}