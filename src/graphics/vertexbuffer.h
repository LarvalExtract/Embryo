#pragma once

#include <GL/glew.h>
#include <maths/vec2.h>
#include <maths/vec3.h>

// Create an OpenGL vertex buffer
// Pass in a pointer to the array of vertex data, and the size of the array
class VertexBuffer
{
public:
	VertexBuffer(const float *data, unsigned int size);
	VertexBuffer(const Vec2<float> *data, unsigned int size);
	VertexBuffer(const Vec3<float> *data, unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();
	void DrawArray(unsigned int renderMode);

private:
	unsigned int m_Count;
	GLuint vbo;
};