#include "vertexbuffer.h"

VertexBuffer::VertexBuffer(const float* data, unsigned int size)
{	
	// The number of elements in the vertex array is required by glDrawArrays
	// Get the number of elements by dividing the size of the vertex array by the size of one element in the array
	m_Count = size / sizeof(float);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Creating an OpenGL buffer requires the size of the array, so just use the value of size
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const Vec2<float> *data, unsigned int size)
{
	// The number of elements in the vertex array is required by glDrawArrays
	// Get the number of elements by dividing the size of the vertex array by the size of one element in the array
	m_Count = size / sizeof(Vec2<float>);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Creating an OpenGL buffer requires the size of the array, so just use the value of size
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const Vec3<float> *data, unsigned int size)
{
	// The number of elements in the vertex array is required by glDrawArrays
	// Get the number of elements by dividing the size of the vertex array by the size of one element in the array
	m_Count = size / sizeof(Vec3<float>);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Creating an OpenGL buffer requires the size of the array, so just use the value of size
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vbo);
}

void VertexBuffer::DrawArray(unsigned int renderMode)
{
	// Draw the vertex array using the count, calculated in the ctor
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