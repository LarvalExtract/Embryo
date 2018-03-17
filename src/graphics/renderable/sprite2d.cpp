#include "sprite2d.h"

Sprite2D::Sprite2D(const std::string &textureName) :
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

	float texCoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	vao.AddBuffer(positions, sizeof(positions), 0, 2);
	vao.AddBuffer(texCoords, sizeof(texCoords), 2, 2);

	shader = new Shader("basicSprite");
}

void Sprite2D::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	vao.Bind();
	shader->Bind();
	shader->SetUniformMat4("mpMatrix", GetTransformMatrix() * camera.projectionMatrix);
	texture->Bind(0);
	vao.DrawArrays(renderMode);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

Texture2D& Sprite2D::GetTexture()
{
	return *texture;
}