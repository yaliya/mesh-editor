#include "shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

Shader::Shader(const char* vertexShaderPath, const char * fragmentShaderPath, const char* geometryShaderPath)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
	std::string geometryShaderCode;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	std::ifstream geometryShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderCode = vertexShaderStream.str();
		fragmentShaderCode = fragmentShaderStream.str();

		if (geometryShaderPath != nullptr) {
			geometryShaderFile.open(geometryShaderPath);
			std::stringstream geometryShaderStream;

			geometryShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();

			geometryShaderCode = geometryShaderStream.str();
		}
	}
	catch (std::ifstream::failure&)
	{
		throw std::runtime_error("ERROR::SHADER::FILE_NOT_READ");
	}

	int success;
	char infoLog[512];

	const char* vertexShaderSource = vertexShaderCode.c_str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	glGetProgramInfoLog(program, 512, nullptr, infoLog);

	if (!success) {
		throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
	}

	unsigned int geometry;

	if (geometryShaderPath != nullptr) {
		const char* geometryShaderSource = geometryShaderCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryShaderSource, nullptr);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << infoLog << std::endl;
			throw std::runtime_error("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED");
		}
	}

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << infoLog << std::endl;
		throw std::runtime_error("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED");
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);

	if (geometryShaderPath != nullptr) {
		glAttachShader(program, geometry);
	}

	glAttachShader(program, fragment);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
	}

	glDeleteShader(vertex);

	if (geometryShaderPath != nullptr) {
		glDeleteShader(geometry);
	}

	glDeleteShader(fragment);
}

Shader::~Shader()= default;

void Shader::bind() const
{
	glUseProgram(program);
}

GLuint Shader::getProgram() const
{
	return program;
}

void Shader::setInt(const std::string & uniform, const int value) const
{
	glUniform1i(glGetUniformLocation(program, uniform.c_str()), value);
}

void Shader::setBool(const std::string & uniform, const bool value) const
{
	glUniform1i(glGetUniformLocation(program, uniform.c_str()), static_cast<int>(value));
}

void Shader::setFloat(const std::string & uniform, const float value) const
{
	glUniform1f(glGetUniformLocation(program, uniform.c_str()), value);
}