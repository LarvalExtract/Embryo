#pragma once
#include <string>
#include <GL/glew.h>

#include <maths/vec2.h>
#include <maths/vec3.h>
#include <maths/vec4.h>
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

	void SetUniformInt  (const std::string& name, const int& value);
	void SetUniformVec2i(const std::string& name, const int& x, const int& y);
	void SetUniformVec3i(const std::string& name, const int& x, const int& y, const int& z);
	void SetUniformVec4i(const std::string& name, const int& x, const int& y, const int& z, const int& w);

	void SetUniformInt_u  (const std::string& name, const unsigned int& value);
	void SetUniformVec2i_u(const std::string& name, const unsigned int& x, const unsigned int& y);
	void SetUniformVec3i_u(const std::string& name, const unsigned int& x, const unsigned int& y, const unsigned int& z);
	void SetUniformVec4i_u(const std::string& name, const unsigned int& x, const unsigned int& y, const unsigned int& z, const unsigned int& w);

	void SetUniformFloat(const std::string& name, const float& value);
	void SetUniformVec2f(const std::string& name, const vec2& vector);
	void SetUniformVec3f(const std::string& name, const vec3& vector);
	void SetUniformVec4f(const std::string& name, const vec4& vector);

	void SetUniformMat3(const std::string& name, const mat3& matrix);
	void SetUniformMat4(const std::string& name, const mat4& matrix);

private:
	GLuint glsl_program;
	GLuint vertexShader, fragmentShader;

	std::string name;
};
