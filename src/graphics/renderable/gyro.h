#pragma once

#include "renderable.h"

// Create a 3D wireframe gyro object
class Gyro : public Renderable
{
public:
	Gyro(float radius, char segments);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
};