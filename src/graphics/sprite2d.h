#pragma once

#include "Renderable3D.h"

// Render 2D sprites from textures
class Sprite2D : public Renderable3D
{
public:
	Sprite2D(const std::string& filePath);

	void Draw() override;

private:
	Texture2D *texture;
};