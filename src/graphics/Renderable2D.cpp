#include "Renderable2D.h"

Renderable2D::Renderable2D(unsigned int renderMode) :
	position(0, 0, 0),
	rotation(0, 0, 1),
	scale(1, 1, 1),
	origin(0.0f, 0.0f, 0.0f),
	translateMatrix(mat4::Translation(position)),
	rotateMatrix(mat4::Rotation(0, rotation)),
	scaleMatrix(mat4::Scale(scale)),
	renderMode(renderMode),
	shader(nullptr)
{

}

Renderable2D::~Renderable2D()
{

}

Shader& Renderable2D::GetShader()
{
	return *shader;
}

void Renderable2D::SetPosition(float x, float y)
{
	position.x = x;// -origin.x;
	position.y = y;// -origin.y;

	translateMatrix = mat4::Translation(position);// *mat4::Translation(origin);
}

void Renderable2D::SetRotation(float z)
{	
	rotateMatrix = mat4::Rotation(z, rotation);
}
	 
void Renderable2D::SetScale(float x, float y)
{
	scale.x = x;
	scale.y = y;

	scaleMatrix = mat4::Scale(scale);
}

void Renderable2D::SetOrigin(float x, float y)
{
	origin.x = x;
	origin.y = y;
}
	 
void Renderable2D::Draw()
{
	vao.Bind();
	shader->Bind();
	vao.DrawElements(renderMode);
}



Sprite::Sprite(const std::string& texturePath) :
	Renderable2D(GL_QUADS)
{
	texture = new Texture2D(texturePath);

	width = texture->GetWidth();
	height = texture->GetHeight();

	//origin.x = -width / 2.0f;
	//origin.y = -height / 2.0f;

	float positions[] = {
		 0.0f,	height,
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

	unsigned short indices[] = {
		0, 1, 2, 3
	};

	vao.AddBuffers(positions, sizeof(positions), 0, 2);
	vao.AddBuffers(texCoords, sizeof(texCoords), 2, 2);
	vao.AddIndices(indices, 4);

	shader = new Shader("basicSprite");
}

void Sprite::SetTexture(const std::string& texturePath)
{
	delete texture;

	texture = new Texture2D(texturePath);
}

void Sprite::CenterOrigin()
{
	origin.x = -width / 2.0f;
	origin.y = -height / 2.0f;
}

void Sprite::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vao.Bind();
	texture->Bind(0);
	shader->Bind();
	vao.DrawElements(renderMode);

	glDisable(GL_BLEND);
}



Shape2D::Shape2D::Shape2D(unsigned int renderMode) :
	Renderable2D::Renderable2D(renderMode),
	colour(1, 1, 1, 1)
{
	shader = new Shader("basic2D");
}

void Shape2D::Shape2D::SetColour(float r, float g, float b, float a)
{
	colour.x = r;
	colour.y = g;
	colour.z = b;
	colour.w = a;

	shader->Bind();
	shader->SetUniformVec4f("colour", colour);
}



Shape2D::Quad::Quad(float width, float height) :
	Shape2D::Shape2D(GL_QUADS),
	width(width),
	height(height)	
{
	float positions[] = {
		 0.0f,   height,
		 0.0f,   0.0f,
		 width,  0.0f,
		 width,  height
	};

	unsigned short indices[] = {
		0, 1, 2, 3
	};

	origin.x = width / 2.0f;
	origin.y = height / 2.0f;

	vao.AddBuffers(positions, sizeof(positions), 0, 2);
	vao.AddIndices(indices, 4);

	shader->Bind();
	shader->SetUniformVec4f("colour", colour);
}




Shape2D::Triangle::Triangle(float size) :
	Shape2D::Shape2D(GL_TRIANGLES)
{
	float positions[] = {
		 size / 2.0f,  size / 2.0f,
		-size / 2.0f, -size / 2.0f,
		 size / 2.0f, -size / 2.0f
	};

	vao.AddBuffers(positions, sizeof(positions), 0, 2);

	shader->Bind();
	shader->SetUniformVec4f("colour", colour);
}

void Shape2D::Triangle::Draw()
{
	vao.Bind();
	shader->Bind();
	vao.DrawArrays(renderMode);
}