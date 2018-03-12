#pragma once

#include <GL/glew.h>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void AddData(const void* data, unsigned int size);
	void DrawArray(unsigned int renderMode);

	void Bind();
	void Unbind();
private:
	unsigned int m_Size;
	GLuint vbo;
};