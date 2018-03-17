#pragma once

#include "renderable.h"
#include <graphics/texture.h>

class Sprite2D : public Renderable
{
public:
	Sprite2D(const std::string &textureName);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
	Texture2D& GetTexture();

private:
	Texture2D *texture;
};