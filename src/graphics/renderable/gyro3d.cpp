#include "gyro3d.h"

Gyro3D::Gyro3D(float radius, char segments) :
	Renderable(GL_LINES)
{
	name = "Gyro";

	std::vector<Vec3<float>> positions;
	std::vector<unsigned short> indices;

	// Segments must not be less than 4
	if (segments < 4)
		segments = 4;
	
	// Algorithm doesn't currently work with odd numbered segments
	else
		if (segments % 2 != 0)
			segments++;

	positions.reserve(segments * 3);
	indices.reserve(segments * 6);

	// Generate X-aligned circle co-ordinates
	for (float i = 0; i < 360.0f; i += 360.0f / segments)
	{
		positions.push_back(
			Vec3<float>(0.0f,								// X
				radius * sin(Maths::Radians(i)),			// Y	   
				radius * cos(Maths::Radians(i))));			// Z
	}

	// Generate Y-aligned circle co-ordinates
	for (float i = 0; i < 360.0f; i += 360.0f / segments)
	{
		positions.push_back(
			Vec3<float>(radius * sin(Maths::Radians(i)),	// X
				0.0f,										// Y	   
				radius * cos(Maths::Radians(i))));			// Z
	}

	// Generate Z-aligned circle co-ordinates
	for (float i = 0; i < 360.0f; i += 360.0f / segments)
	{
		positions.push_back(
			Vec3<float>(radius * sin(Maths::Radians(i)),	// X
				radius * cos(Maths::Radians(i)),			// Y	   
				0.0f));										// Z
	}

	// Generate gyro indices
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

	vao.AddBuffer(&positions[0], sizeof(positions[0]) * positions.size(), 0, 3);
	vao.AddIndices(&indices[0], indices.size());

	shader = new Shader("basiccolour");
	shader->Bind();
	shader->SetUniformVec4f("u_colour", colour);
}

void Gyro3D::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	vao.Bind();
	shader->Bind();
	shader->SetUniformMat4("mvpMatrix", GetTransformMatrix() * cameraMatrix);
	vao.DrawElements(renderMode);
}