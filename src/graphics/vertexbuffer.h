#pragma once

#include <GL/glew.h>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();
	void DrawArray(unsigned int renderMode);

private:
	unsigned int m_Count;
	GLuint vbo;
};