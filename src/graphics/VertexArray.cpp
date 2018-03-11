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

void VertexArray::AddBuffers(const void* data, unsigned int size, const unsigned int& attribIndex, const unsigned int& attribCount)
{
	VertexBuffer* buffer = new VertexBuffer;
	buffer->AddData(data, size);
	vertexBuffers.push_back(buffer);

	glVertexAttribPointer(attribIndex, attribCount, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribIndex);
}

void VertexArray::AddIndices(const void* data, const unsigned int& count)
{
	IndexBuffer* indices = new IndexBuffer;
	indices->AddData(data, count);
	indexBuffers.push_back(indices);
}

void VertexArray::DrawArrays(const unsigned int& renderMode)
{
	vertexBuffers[0]->DrawArray(renderMode);
}

void VertexArray::DrawElements(const unsigned int& renderMode)
{
	for (char i = 0; i < indexBuffers.size(); i++)
		indexBuffers[i]->DrawElements(renderMode);
}