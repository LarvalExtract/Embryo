#include "shader.h"
#include <iostream>
#include <utilities/file.h>


static GLuint CreateShader(const std::string& shaderSource, GLenum shaderType);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

Shader::Shader()
{
}

Shader::Shader(const std::string& fileName) :
	name(fileName)
{
	Init(fileName);
}

Shader::~Shader()
{
	glDetachShader(glsl_program, vertexShader);
	glDeleteShader(vertexShader);

	glDetachShader(glsl_program, fragmentShader);
	glDeleteShader(fragmentShader);
	
	glDeleteProgram(glsl_program);
}

void Shader::Init(const std::string& shaderPath)
{
	glsl_program = glCreateProgram();

	// Get vertex and fragment shader source code
	vertexShader   = CreateShader(File::Load("res/shaders/" + shaderPath + ".vert"), GL_VERTEX_SHADER);
	fragmentShader = CreateShader(File::Load("res/shaders/" + shaderPath + ".frag"), GL_FRAGMENT_SHADER);

	glAttachShader(glsl_program, vertexShader);
	glAttachShader(glsl_program, fragmentShader);
	
	glBindAttribLocation(glsl_program, 0, "in_position");	// Vertex position attribute
	glBindAttribLocation(glsl_program, 1, "in_normal");		// Vertex normal attribute
	glBindAttribLocation(glsl_program, 2, "in_texCoord");	// Vertex UV co-ordinate attribute
	glBindAttribLocation(glsl_program, 3, "in_colour");		// Vertex colour attribute

	glLinkProgram(glsl_program);
	CheckShaderError(glsl_program, GL_LINK_STATUS, true, "Error: Shader program linking failed: ");

	glValidateProgram(glsl_program);
	CheckShaderError(glsl_program, GL_VALIDATE_STATUS, true, "Error: Invalid shader program: ");
}

void Shader::Bind()
{
	glUseProgram(glsl_program);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	return glGetUniformLocation(glsl_program, name.c_str());
}

std::string Shader::GetName()
{
	return name;
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformVec2i(const std::string &name, int x, int y)
{
	glUniform2i(GetUniformLocation(name), x, y);
}

void Shader::SetUniformVec3i(const std::string &name, int x, int y, int z)
{
	glUniform3i(GetUniformLocation(name), x, y, z);
}

void Shader::SetUniformVec4i(const std::string &name, int x, int y, int z, int w)
{
	glUniform4i(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetUniformInt_u(const std::string &name, unsigned int value)
{
	glUniform1ui(GetUniformLocation(name), value);
}

void Shader::SetUniformVec2i_u(const std::string &name, unsigned int x, unsigned int y)
{
	glUniform2ui(GetUniformLocation(name), x, y);		 
}														 
														 
void Shader::SetUniformVec3i_u(const std::string &name, unsigned int x, unsigned int y, unsigned int z)
{		
	glUniform3ui(GetUniformLocation(name), x, y, z);	
}														
														
void Shader::SetUniformVec4i_u(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w)
{
	glUniform4ui(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetUniformFloat(const std::string &name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
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

	if (shader == 0)
		std::cerr << "Error: Couldn't create shader!" << std::endl;

	const GLchar* shaderSource = source.c_str();
	const GLint shaderSourceLength = source.length();

	glShaderSource(shader, 1, &shaderSource, &shaderSourceLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

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

		std::cerr << errorMessage << ": " << error << std::endl;
	}
}