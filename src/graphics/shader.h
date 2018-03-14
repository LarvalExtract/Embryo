#pragma once
#include <string>
#include <GL/glew.h>

#include <maths/Vec2.h>
#include <maths/Vec3.h>
#include <maths/Vec4.h>
#include <maths/mat3.h>
#include <maths/mat4.h>

class Shader
{
public:
	// Do NOT include file extension in shader file path!!!
	Shader();
	Shader(const std::string& fileName);
	~Shader();

	void Init(const std::string& shaderPath);
	void Bind();

	unsigned int GetUniformLocation(const std::string& name);

	std::string GetName();

	void SetUniformInt  (const std::string &name, int value);
	void SetUniformVec2i(const std::string &name, int x, int y);
	void SetUniformVec3i(const std::string &name, int x, int y, int z);
	void SetUniformVec4i(const std::string &name, int x, int y, int z, int w);

	void SetUniformInt_u  (const std::string &name, unsigned int value);
	void SetUniformVec2i_u(const std::string &name, unsigned int x, unsigned int y);
	void SetUniformVec3i_u(const std::string &name, unsigned int x, unsigned int y, unsigned int z);
	void SetUniformVec4i_u(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w);

	void SetUniformFloat(const std::string &name, float value);
	void SetUniformVec2f(const std::string &name, const Vec2<float> &vector);
	void SetUniformVec3f(const std::string &name, const Vec3<float> &vector);
	void SetUniformVec4f(const std::string &name, const Vec4<float> &vector);

	void SetUniformMat3(const std::string &name, const mat3& matrix);
	void SetUniformMat4(const std::string &name, const Mat4& matrix);

private:
	GLuint glsl_program;
	GLuint vertexShader, fragmentShader;

	std::string name;
};
