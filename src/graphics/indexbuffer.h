#pragma once

#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();
	void DrawElements(unsigned int renderMode);

private:
	GLuint ibo;
	unsigned int m_Count;
};
