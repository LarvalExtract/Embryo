#include "vertexbuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	m_Count = size / sizeof(float);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vbo);
}

void VertexBuffer::DrawArray(unsigned int renderMode)
{
	glDrawArrays(renderMode, 0, m_Count);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}