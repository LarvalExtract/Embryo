#pragma once

#include "renderable.h"
#include <graphics/texture2d.h>

class Sprite : public Renderable
{
public:
	Sprite() : Renderable(GL_TRIANGLES) {};
	Sprite(const std::string &textureName, bool resize);
	Sprite(unsigned int width, unsigned int height, unsigned char* &data);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
	Texture2D& GetTexture();

private:
	Texture2D *texture;
};