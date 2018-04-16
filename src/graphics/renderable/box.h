#pragma once

#include "renderable.h"

// Create a simple 3D box
class Box : public Renderable
{
public:
	Box(float width, float height, float depth);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

};