#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>

// Create an OpenGL vertex array
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();
	void AddBuffer(const float *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount);
	void AddBuffer(const Vec2<float> *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount);
	void AddBuffer(const Vec3<float> *data, unsigned int size, unsigned int attribIndex, unsigned int attribCount);
	void AddIndices(const unsigned short *data, unsigned int count);
	void AddIndices(const unsigned int *data, unsigned int count);		// Overloaded so that index arrays can be shorts or ints
	void DrawArrays(unsigned int renderMode);
	void DrawElements(unsigned int renderMode);

private:
	GLuint vao;
	std::vector<VertexBuffer*> vertexBuffers;
	std::vector<IndexBuffer*>  indexBuffers;
};