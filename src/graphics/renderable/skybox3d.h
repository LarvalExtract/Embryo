#pragma once

#include "renderable.h"
#include <graphics/texture.h>

class Skybox3D : public Renderable
{
public:
	Skybox3D(const std::string &skyboxName, float size);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

private:
	Cubemap *skyTexture;
};