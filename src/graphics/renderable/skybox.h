#pragma once

#include "renderable.h"
#include <graphics/cubemap.h>

class Skybox : public Renderable
{
public:
	Skybox(const std::string &skyboxName, float size);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

private:
	Cubemap *skyTexture;
};