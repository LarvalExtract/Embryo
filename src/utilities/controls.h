#pragma once
#include <graphics/camera.h>
#include <graphics/window.h>

#include <string>
#include <maths\vec2.h>

class Controls
{
public:
	Controls();
	~Controls();

	void MatricesFromInputs(Window &window, Camera &camera, double deltaTime);
private:
	Vec2<double> lookAngle;

	float sensitivity;
	float movementSpeed;
	bool bEnableCamera;
};