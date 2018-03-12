#include "Renderable3D.h"

#include <utilities/import_model.h>

//// Base class methods
// Base constructor
// This constructor is called by child classes to initialise inherited variables
Renderable3D::Renderable3D(unsigned int renderMode) :
	position(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	translationMatrix(mat4::Translation(position)),
	rotationMatrix(mat4::Rotation(0, rotation)),
	scaleMatrix(mat4::Scale(scale)),
	renderMode(renderMode),
	specularity(0),
	glossiness(1)
{
	//shader = new Shader("test");						// Change this so that default shader/texture reuse the same pointer for each Renderable3D object,
	//texture = new Texture2D("./resources/bitmaps/missingtexture2.tga");		// rather than creating a new shader/texture
}

Renderable3D::~Renderable3D()
{
	//if (texture != nullptr)
	//	delete texture;
	//
	//if (shader != nullptr)
	//	delete shader;
}

void Renderable3D::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	translationMatrix = mat4::Translation(position);
}

void Renderable3D::SetRot(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	rotationMatrix = mat4::Rotation(rotation.z, 
		Vec3<float>(0.0f, 0.0f, 1.0f)) * mat4::Rotation(rotation.y, Vec3<float>(0.0f, 1.0f, 0.0f)) * mat4::Rotation(rotation.x, Vec3<float>(1.0f, 0.0f, 0.0f));
}

void Renderable3D::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	scaleMatrix = mat4::Scale(scale);
}

void Renderable3D::SetShader(Shader *pShader)
{
	shader = pShader;
}

void Renderable3D::SetRenderMode(unsigned int mode)
{
	renderMode = mode;
}

void Renderable3D::SetSpecularity(float value)
{
	specularity = value;
}

void Renderable3D::SetGlossiness(float value)
{
	glossiness = value;
}
void Renderable3D::SetName(const std::string& name)
{
	this->name = name;
}

Vec3<float> Renderable3D::GetPos()
{
	return position;
}

Vec3<float> Renderable3D::GetRot()
{
	return rotation;
}

Vec3<float> Renderable3D::GetScale()
{
	return scale;
}

std::string Renderable3D::GetName()
{
	return name;
}

Texture& Renderable3D::GetTexture()
{
	return *texture;
}

Shader& Renderable3D::GetShader()
{
	return *shader;
}

float Renderable3D::GetSpecularity()
{
	return specularity;
}

float Renderable3D::GetGlossiness()
{
	return glossiness;
}

void Renderable3D::Draw()
{
	vao.Bind();
	shader->Bind();
	texture->Bind(0);
	vao.DrawElements(renderMode);
}

// Model3D sub-class methods

// Default directory: res/models/
Model3D::Model3D(const std::string& filePath) :
	Renderable3D(GL_TRIANGLES)
{
	name = filePath.substr(0, filePath.rfind("."));

	std::vector<Vec3<float>> positions, normals;
	std::vector<Vec2<float>> texCoords;
	std::vector<unsigned short> indices;

	ImportModel(filePath, positions, normals, texCoords, indices);

	vao.AddBuffers(&positions[0], positions.size() * sizeof(positions[0]), 0, 3);
	vao.AddBuffers(&normals[0],	  normals.size()   * sizeof(normals[0]),   1, 3);
	vao.AddBuffers(&texCoords[0], texCoords.size() * sizeof(texCoords[0]), 2, 2);
	vao.AddIndices(&indices[0],   indices.size());
}

void Model3D::SetTexture(const std::string& filePath)
{
	texture = new Texture2D(filePath);
}

// Sprite2D sub-class methods

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

	vao.Bind();
	shader->Bind();
	texture->Bind(0);
	vao.DrawElements(renderMode);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}


// Skybox methods
// Inherits from Renderable3D

// Create skybox using name of skybox texture
// Default directory: res/bitmaps/sky/
Skybox::Skybox(const std::string& skyboxName) :
	Renderable3D(GL_QUADS), scale(100)
{
	name = skyboxName;

	float positions[] = {
	//   X		 Y		 Z
		-scale,  scale, -scale,		// 0
		 scale,  scale, -scale,		// 1
		 scale,  scale,  scale,		// 2
		-scale,  scale,  scale,		// 3	 													 	
		-scale, -scale, -scale,		// 4
		 scale, -scale, -scale,		// 5
		 scale, -scale,  scale,		// 6
		-scale, -scale,  scale		// 7
	};

	unsigned short indices[] = {
		0, 1, 2, 3,		// Top,		Y+
		7, 4, 0, 3,		// Left,	X-
		6, 7, 3, 2,		// Front,	Z-
		5, 6, 2, 1,		// Right,	X+
		4, 5, 1, 0,		// Back,	Z+
		7, 6, 5, 4		// Bottom,	Y-
	};

	// Send skybox vertex data to GPU
	vao.AddBuffers(positions, sizeof(positions), 0, 3);
	vao.AddIndices(indices, 24);

	// Use default sky shader
	shader = new Shader("sky");

	// Apply cubemap texture to skybox
	texture = new Cubemap("sky/" + skyboxName);
}


//// Primitive methods

void Primitive::Primitive::SetColour(float red, float green, float blue, float alpha)
{
	colour.x = red;
	colour.y = green;
	colour.z = blue;
	colour.w = alpha;

	shader->SetUniformVec4f("wire_colour", colour);
}

void Primitive::Primitive::Draw()
{
	vao.Bind();
	shader->Bind();

	vao.DrawElements(renderMode);
}

// Box methods
// Inherits from Primitive base class
Primitive::Box::Box(float width, float height, float depth, const Vec4<float> &colour) :
	Primitive(GL_LINES, colour)
{
	name = "Box";

	float positions[] = {
		//   X		 Y		 Z
		-width,  height, -depth,	// 0
		 width,  height, -depth,	// 1
		 width,  height,  depth,	// 2
		-width,  height,  depth,	// 3	 													 	
		-width, -height, -depth,	// 4
		 width, -height, -depth,	// 5
		 width, -height,  depth,	// 6
		-width, -height,  depth		// 7
	};

	unsigned short indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,		// Top
		0, 4, 1, 5, 2, 6, 3, 7,		// Sides
		4, 5, 5, 6, 6, 7, 7, 4		// Bottom
	};

	vao.AddBuffers(positions, sizeof(positions), 0, 3);
	vao.AddIndices(indices, 24);

	shader = new Shader("basiccolour");
	shader->Bind();
	shader->SetUniformVec4f("wire_colour", colour);
}

// Sphere methods
// Inherits from Primitive base class
Primitive::Sphere::Sphere(unsigned char segments, float radius, const Vec4<float> &colour) :
	Primitive(GL_LINES, colour)
{
	name = "Sphere";

	std::vector<Vec3<float>> positions;
	std::vector<unsigned short> indices;

	// Algorithm doesn't currently work with odd numbered segments. This converts segments to even
	if (static_cast<float>(segments) / 2 - (segments / 2) > 0.0001f)
		segments++;

	positions.reserve(segments * 3);
	indices.reserve(segments * 6);

	// Generate X-aligned circle co-ordinates
	for (float i = 0; i < 360.0f; i += 360.0f / segments)
	{
		positions.push_back(
			Vec3<float>(0.0f,							// X
				radius * sin(radians(i)),		// Y	   
				radius * cos(radians(i))));		// Z
	}

	// Generate Y-aligned circle co-ordinates
	for (float i = 0; i < 360.0f; i += 360.0f / segments)
	{
		positions.push_back(
			Vec3<float>(radius * sin(radians(i)),		// X
				 0.0f,							// Y	   
				 radius * cos(radians(i))));	// Z
	}

	// Generate Z-aligned circle co-ordinates
	for (float i = 0; i < 360.0f; i += 360.0f / segments)
	{
		positions.push_back(
			Vec3<float>(radius * sin(radians(i)),		// X
				 radius * cos(radians(i)),		// Y	   
				 0.0f));						// Z
	}

	// Generate sphere indices
	for (unsigned char j = 0; j < 3; j++)
	{
		for (unsigned char i = 0; i < segments; i++)
		{
			if (i != (segments - 1))
			{
				indices.push_back((segments * j) + i);
				indices.push_back((segments * j) + i + 1);
			}
			else
			{
				indices.push_back((segments * j) + i);
				indices.push_back(indices.at((segments * j) * 2));
			}
		}
	}

	vao.AddBuffers(&positions[0], sizeof(positions[0]) * positions.size(), 0, 3);
	vao.AddIndices(&indices[0], indices.size());

	shader = new Shader("basiccolour");
	shader->Bind();
	shader->SetUniformVec4f("wire_colour", colour);
}

// Pivot sub-class methods
// Inherits from Primitive base class
Primitive::Pivot::Pivot() :
	Primitive(GL_LINES, Vec4<float>(1, 1, 1, 1))
{
	name = "Pivot";

	float positions[] = {
		0.0f, 0.0f, 0.0f,			// Origin
		1.0f, 0.0f, 0.0f,			// X

		0.0f, 0.0f, 0.0f,			// Origin
		0.0f, 1.0f, 0.0f,			// Y

		0.0f, 0.0f, 0.0f,			// Origin
		0.0f, 0.0f, -1.0f			// Z
	};

	float colours[] = {
		1.0f, 0.0f, 0.0f, 1.0f,		// Red
		1.0f, 0.0f, 0.0f, 1.0f,		// Red
		0.0f, 1.0f, 0.0f, 1.0f,		// Green
		0.0f, 1.0f, 0.0f, 1.0f,		// Green
		0.0f, 0.0f, 1.0f, 1.0f,		// Blue
		0.0f, 0.0f, 1.0f, 1.0f		// Blue
	};

	vao.AddBuffers(positions, sizeof(positions), 0, 3);
	vao.AddBuffers(colours, sizeof(colours), 3, 4);

	shader = new Shader("in_colour");
}

void Primitive::Pivot::Draw()
{
	vao.Bind();
	shader->Bind();

	vao.DrawArrays(renderMode);
}

