#pragma once

#include "renderable.h"
#include <graphics/material.h>
#include <vector>

class Model : public Renderable
{
public:
	Model();
	~Model();

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

	Material material;

protected:

};