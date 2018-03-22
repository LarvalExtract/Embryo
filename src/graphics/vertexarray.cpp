#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

VertexArray::~VertexArray()
{
	for (char i = 0; i < vertexBuffers.size(); i++)
		delete vertexBuffers[i];

	for (char i = 0; i < indexBuffers.size(); i++)
		delete indexBuffers[i];

	glDeleteVertexArrays(1, &vao);
}

void VertexArray::Bind()
{
	glBindVertexArray(vao);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const float *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount)
{
	vertexBuffers.push_back(new VertexBuffer(data, size));
	glVertexAttribPointer(attribIndex, attribCount, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribIndex);
}

void VertexArray::AddBuffer(const Vec2<float> *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount)
{
	vertexBuffers.push_back(new VertexBuffer(data, size));
	glVertexAttribPointer(attribIndex, attribCount, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribIndex);
}

void VertexArray::AddBuffer(const Vec3<float> *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount)
{
	vertexBuffers.push_back(new VertexBuffer(data, size));
	glVertexAttribPointer(attribIndex, attribCount, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribIndex);
}

void VertexArray::AddIndices(const unsigned short* data, unsigned int count)
{
	indexBuffers.push_back(new IndexBuffer(data, count));
}

void VertexArray::AddIndices(const unsigned int* data, unsigned int count)
{
	indexBuffers.push_back(new IndexBuffer(data, count));
}

void VertexArray::AddIndices(const std::vector<unsigned int> &vector)
{
	indexBuffers.push_back(new IndexBuffer(vector.data(), vector.size()));
}

void VertexArray::DrawArrays(unsigned int renderMode)
{
	for (char i = 0; i < vertexBuffers.size(); i++)
		vertexBuffers[i]->DrawArray(renderMode);
}

void VertexArray::DrawElements(unsigned int renderMode)
{
	for (char i = 0; i < indexBuffers.size(); i++)
		indexBuffers[i]->DrawElements(renderMode);
}