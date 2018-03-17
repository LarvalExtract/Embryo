#pragma once

#include "renderable.h"

// Create a transform gizmo
class Gizmo3D : public Renderable
{
public:
	Gizmo3D();

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
};