#include "shader.h"
#include <iostream>
#include <utilities/file.h>
#include <utilities/console.h>

static GLuint CreateShader(const std::string& shaderSource, GLenum shaderType);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

Shader::Shader(const std::string& fileName) :
	name(fileName)
{
	hShaderProgram = glCreateProgram();

	// Get vertex and fragment shader source code
	hVertexShader = CreateShader(File::Load("res/shaders/" + fileName + ".vert"), GL_VERTEX_SHADER);
	hFragmentShader = CreateShader(File::Load("res/shaders/" + fileName + ".frag"), GL_FRAGMENT_SHADER);

	glAttachShader(hShaderProgram, hVertexShader);
	glAttachShader(hShaderProgram, hFragmentShader);

	glBindAttribLocation(hShaderProgram, 0, "in_position");	// Vertex position attribute
	glBindAttribLocation(hShaderProgram, 1, "in_normal");		// Vertex normal attribute
	glBindAttribLocation(hShaderProgram, 2, "in_texCoord");	// Vertex UV co-ordinate attribute
	glBindAttribLocation(hShaderProgram, 3, "in_colour");		// Vertex colour attribute

	glLinkProgram(hShaderProgram);
	CheckShaderError(hShaderProgram, GL_LINK_STATUS, true, "Shader program linking failed: ");

	glValidateProgram(hShaderProgram);
	CheckShaderError(hShaderProgram, GL_VALIDATE_STATUS, true, "Invalid shader program: ");
}

Shader::~Shader()
{
	glDetachShader(hShaderProgram, hVertexShader);
	glDeleteShader(hVertexShader);

	glDetachShader(hShaderProgram, hFragmentShader);
	glDeleteShader(hFragmentShader);
	
	glDeleteProgram(hShaderProgram);
}

void Shader::Bind()
{
	glUseProgram(hShaderProgram);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	return glGetUniformLocation(hShaderProgram, name.c_str());
}

std::string Shader::GetName()
{
	return name;
}

int Shader::GetUniformInt(const std::string &name)
{
	int uniform;
	glGetUniformiv(hShaderProgram, GetUniformLocation(name), &uniform);
	return uniform;
}

unsigned int Shader::GetUniformInt_u(const std::string &name)
{
	unsigned int uniform;
	glGetUniformuiv(hShaderProgram, GetUniformLocation(name), &uniform);
	return uniform;
}

float Shader::GetUniformFloat(const std::string &name)
{
	float uniform;
	glGetUniformfv(hShaderProgram, GetUniformLocation(name), &uniform);
	return uniform;
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformInt_u(const std::string &name, unsigned int value)
{
	glUniform1ui(GetUniformLocation(name), value);
}

void Shader::SetUniformFloat(const std::string &name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformVec2i(const std::string &name, const Vec2<int> &vector)
{
	glUniform2i(GetUniformLocation(name), vector.x, vector.y);
}

void Shader::SetUniformVec3i(const std::string &name, const Vec3<int> &vector)
{
	glUniform3i(GetUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::SetUniformVec4i(const std::string &name, const Vec4<int> &vector)
{
	glUniform4i(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetUniformVec2ui(const std::string &name, const Vec2<unsigned int> &vector)
{
	glUniform2ui(GetUniformLocation(name), vector.x, vector.y);
}

void Shader::SetUniformVec3ui(const std::string &name, const Vec3<unsigned int> &vector)
{
	glUniform3ui(GetUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::SetUniformVec4ui(const std::string &name, const Vec4<unsigned int> &vector)
{
	glUniform4ui(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetUniformVec2f(const std::string &name, const Vec2<float> &vector)
{
	glUniform2f(GetUniformLocation(name), vector.x, vector.y);
}

void Shader::SetUniformVec3f(const std::string &name, const Vec3<float> &vector)
{
	glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::SetUniformVec4f(const std::string &name, const Vec4<float> &vector)
{
	glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetUniformMat3(const std::string &name, const mat3& matrix)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix.elements[0][0]);
}

void Shader::SetUniformMat4(const std::string &name, const Mat4 &matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix.elements[0][0]);
}

static GLuint CreateShader(const std::string &source, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	// TO-DO: Handle error
	if (shader == 0)
		Console::Log(LogType::Error) << "Couldn't create shader!\n";

	const GLchar* shaderSource = source.c_str();
	const GLint shaderSourceLength = source.length();

	glShaderSource(shader, 1, &shaderSource, &shaderSourceLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Shader compilation failed: ");

	return shader;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);

	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);

		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		Console::Log(LogType::Error) << errorMessage << ": " << error << "\n";
	}
}