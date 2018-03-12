#pragma once

#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void AddData(const void* data, unsigned int count);
	void DrawElements(unsigned int renderMode);

	void Bind();
	void Unbind();
private:
	GLuint ibo;

	unsigned int m_Count;
};
