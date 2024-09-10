#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	std::string VertexCode;
	std::string FragmentCode;
	std::ifstream VShaderFile;
	std::ifstream FShaderFile;

	VShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		VShaderFile.open(VertexPath);
		FShaderFile.open(FragmentPath);
		std::stringstream VShaderStream, FShaderStream;
		VShaderStream << VShaderFile.rdbuf();
		FShaderStream << FShaderFile.rdbuf();
		VShaderFile.close();
		FShaderFile.close();
		VertexCode = VShaderStream.str();
		FragmentCode = FShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

	const char* VShaderCode = VertexCode.c_str();
	const char* FShaderCode = FragmentCode.c_str();

	unsigned int Vertex, Fragment;
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vertex, 1, &VShaderCode, nullptr);
	glCompileShader(Vertex);
	checkCompileErrors(Vertex, "VERTEX");

	Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fragment, 1, &FShaderCode, nullptr);
	glCompileShader(Fragment);
	checkCompileErrors(Fragment, "FRAGMENT");

	Id = glCreateProgram();
	glAttachShader(Id, Vertex);
	glAttachShader(Id, Fragment);
	glLinkProgram(Id);
	checkCompileErrors(Id, "PROGRAM");

	glDeleteShader(Vertex);
	glDeleteShader(Fragment);
}

void Shader::use() const
{
	glUseProgram(Id);
}

void Shader::setBool(const std::string& Name, const bool Value) const
{
	glUniform1i(glGetUniformLocation(Id, Name.c_str()), static_cast<int>(Value));
}

void Shader::setInt(const std::string& Name, const int Value) const
{
	glUniform1i(glGetUniformLocation(Id, Name.c_str()), Value);
}

void Shader::setFloat(const std::string& Name, const float Value) const
{
	glUniform1f(glGetUniformLocation(Id, Name.c_str()), Value);
}

void Shader::setVec3(const std::string& Name, const glm::vec3& Value) const
{
	glUniform3fv(glGetUniformLocation(Id, Name.c_str()), 1, &Value[0]);
}

void Shader::setVec3(const std::string& Name, const float X, const float Y, const float Z) const
{
	glUniform3f(glGetUniformLocation(Id, Name.c_str()), X, Y, Z);
}

void Shader::setMat4(const std::string& Name, const glm::mat4& Mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

void Shader::checkCompileErrors(const unsigned int Shader, const std::string& Type)
{
	int Success;
	char InfoLog[1024];
	if (Type != "PROGRAM")
	{
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(Shader, 1024, nullptr, InfoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << Type << "\n" << InfoLog << "\n";
		}
	}
	else
	{
		glGetProgramiv(Shader, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(Shader, 1024, nullptr, InfoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << InfoLog << "\n";
		}
	}
}
