#pragma once

#include "renderable.h"

// Create a 3D wireframe gyro object
class Gyro3D : public Renderable
{
public:
	Gyro3D(float radius, char segments);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
};