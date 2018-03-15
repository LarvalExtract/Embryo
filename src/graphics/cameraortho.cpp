#include "cameraortho.h"

CamOrtho::CamOrtho(float xLeft, float xRight, float yBottom, float yTop, float zNear, float zFar) :
	Camera(), xLeft(xLeft), xRight(xRight), yBottom(yBottom), yTop(yTop), zNear(zNear), zFar(zFar)
{
	projectionMatrix.Orthographic(xLeft, xRight, yBottom, yTop, zNear, zFar);
	viewMatrix.LookAt(position, position + forwardVector, upwardVector);
}