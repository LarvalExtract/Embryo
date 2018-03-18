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
	Shader(const std::string& fileName);
	~Shader();

	void Bind();

	unsigned int GetUniformLocation(const std::string& name);

	std::string GetName();

	int GetUniformInt(const std::string &name);
	unsigned int GetUniformInt_u(const std::string &name);
	float GetUniformFloat(const std::string &name);

	void SetUniformInt(const std::string &name, int value);
	void SetUniformInt_u(const std::string &name, unsigned int value);
	void SetUniformFloat(const std::string &name, float value);
	void SetUniformVec2i(const std::string &name, const Vec2<int> &vector);
	void SetUniformVec3i(const std::string &name, const Vec3<int> &vector);
	void SetUniformVec4i(const std::string &name, const Vec4<int> &vector);
	void SetUniformVec2ui(const std::string &name, const Vec2<unsigned int> &vector);
	void SetUniformVec3ui(const std::string &name, const Vec3<unsigned int> &vector);
	void SetUniformVec4ui(const std::string &name, const Vec4<unsigned int> &vector);
	void SetUniformVec2f(const std::string &name, const Vec2<float> &vector);
	void SetUniformVec3f(const std::string &name, const Vec3<float> &vector);
	void SetUniformVec4f(const std::string &name, const Vec4<float> &vector);
	void SetUniformMat3(const std::string &name, const mat3& matrix);
	void SetUniformMat4(const std::string &name, const Mat4& matrix);

private:
	std::string name;

	GLuint hShaderProgram;
	GLuint hVertexShader;
	GLuint hFragmentShader;
};
