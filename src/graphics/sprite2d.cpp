#include "sprite2d.h"

// Create sprite using texture name
// Default directory: res/bitmaps/
Sprite2D::Sprite2D(const std::string& filePath) :
	Renderable3D(GL_QUADS)
{
	name = filePath.substr(0, filePath.rfind(".") - 1);
	texture = new Texture2D(filePath);

	float positions[] = {
		-(texture->GetWidth() / 2) * 0.01f,  0.0f,							0.0f,
		(texture->GetWidth() / 2) * 0.01f,  0.0f,							0.0f,
		(texture->GetWidth() / 2) * 0.01f,  texture->GetHeight() * 0.01f,	0.0f,
		-(texture->GetWidth() / 2) * 0.01f,  texture->GetHeight() * 0.01f,	0.0f
	};

	float texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	unsigned short indices[] = {
		0, 1, 2, 3
	};

	vao.AddBuffers(positions, sizeof(positions), 0, 3);
	vao.AddBuffers(texCoords, sizeof(texCoords), 2, 2);
	vao.AddIndices(indices, 4);

}

void Sprite2D::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);

	Renderable3D::Draw();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}
