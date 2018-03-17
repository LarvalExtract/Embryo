#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();
	void AddBuffer(const void *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount);
	void AddIndices(const void *data, unsigned int count);
	void DrawArrays(unsigned int renderMode);
	void DrawElements(unsigned int renderMode);

private:
	GLuint vao;
	std::vector<VertexBuffer*> vertexBuffers;
	std::vector<IndexBuffer*>  indexBuffers;
};