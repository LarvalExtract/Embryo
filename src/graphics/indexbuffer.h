#pragma once

#include <GL/glew.h>

// Create an OpenGL index buffer to more efficiently draw a vertex buffer
// Pass in a pointer to the array of indices and the number of elements in the array (not the size!)
class IndexBuffer
{
public:
	IndexBuffer(const unsigned short* data, unsigned int count);
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();
	void DrawElements(unsigned int renderMode);

private:
	GLuint ibo;
	unsigned int m_Count;
};
