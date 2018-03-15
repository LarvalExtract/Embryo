#include "cameraperspective.h"

CamPerspective::CamPerspective(float fov, float aspectRatio, float zNear, float zFar) :
	Camera(), fov(fov), aspectRatio(aspectRatio), zNear(zNear), zFar(zFar)
{
	projectionMatrix.Perspective(fov, aspectRatio, zNear, zFar);
	viewMatrix.LookAt(position, position + forwardVector, upwardVector);
}

void CamPerspective::SetFOV(float value)
{
	fov = value;
	projectionMatrix.Perspective(fov, aspectRatio, zNear, zFar);
}

void CamPerspective::SetAspectRatio(float value)
{
	aspectRatio = value;
	projectionMatrix.Perspective(fov, aspectRatio, zNear, zFar);
}