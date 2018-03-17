#pragma once

#include "renderable.h"
#include <graphics/texture.h>

class Sprite3D : public Renderable
{
public:
	Sprite3D(const std::string &textureName);
	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

private:
	Texture2D *texture;
};