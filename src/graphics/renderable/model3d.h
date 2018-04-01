#pragma once

#include "renderable.h"
#include <graphics/material.h>
#include <vector>

class Model3D : public Renderable
{
public:
	Model3D();
	~Model3D();

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

	Material material;

protected:

};