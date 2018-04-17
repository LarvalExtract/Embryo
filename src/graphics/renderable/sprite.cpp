#include "Sprite.h"

Sprite::Sprite(const std::string &textureName, bool resize) :
	Renderable(GL_QUADS)
{
	name = textureName.substr(textureName.rfind("/") + 1, textureName.rfind("."));
	texture = new Texture2D(textureName);

	width = texture->GetWidth();
	height = texture->GetHeight();

	float positions[] = {
		0.0f,  height,
		0.0f,  0.0f,
		width, 0.0f,
		width, height
	};

	// Scale down the sprite by 100 if reside is true
	// Suitable for sprites occupying 3D space rather than 2D screen space
	if (resize == true)
	{
		positions[1] /= 100.0f;
		positions[4] /= 100.0f;
		positions[6] /= 100.0f;
		positions[7] /= 100.0f;
	}

	float texCoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	vao.AddBuffer(positions, sizeof(positions), 0, 2);
	vao.AddBuffer(texCoords, sizeof(texCoords), 2, 2);
}

Sprite::Sprite(unsigned int width, unsigned int height, unsigned char* &data) :
	Renderable(GL_QUADS)
{
	texture = new Texture2D(width, height, data);

	this->width = width;
	this->height = height;

	float positions[] = {
		0.0f,  height,
		0.0f,  0.0f,
		width, 0.0f,
		width, height
	};

	float texCoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	vao.AddBuffer(positions, sizeof(positions), 0, 2);
	vao.AddBuffer(texCoords, sizeof(texCoords), 2, 2);
}

void Sprite::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	vao.Bind();
	shader->Bind();
	shader->SetUniformMat4("u_mvpMatrix", GetTransformMatrix() * camera.viewMatrix * camera.projectionMatrix);
	texture->Bind(0);
	vao.DrawArrays(renderMode);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

Texture2D& Sprite::GetTexture()
{
	return *texture;
}