#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned short* data, unsigned int count)
{
	// The number of elements in the index array is required by glDrawElements
	// count is already the number of elements, so we can simply assign count to m_Count
	m_Count = count;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Creating an OpenGL buffer requires the size of the array, so multiply the count by the size of an element inside the count
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	// The number of elements in the index array is required by glDrawElements
	// count is already the number of elements, so we can simply assign count to m_Count
	m_Count = count;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Creating an OpenGL buffer requires the size of the array, so multiply the count by the size of an element inside the count
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &ibo);
}

void IndexBuffer::DrawElements(unsigned int renderMode)
{
	glDrawElements(renderMode, m_Count, GL_UNSIGNED_SHORT, nullptr);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}