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

	void AddBuffers(const void* data, unsigned int size, const unsigned int& attribIndex, const unsigned int& attribCount);
	void AddIndices(const void* data, const unsigned int& count);

	void DrawArrays(const unsigned int& renderMode);
	void DrawElements(const unsigned int& renderMode);
private:
	GLuint vao;

	std::vector<VertexBuffer*> vertexBuffers;
	std::vector<IndexBuffer*>  indexBuffers;
};