#pragma once

#include "renderable.h"

// Create a transform gizmo
class Gizmo : public Renderable
{
public:
	Gizmo();

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
};