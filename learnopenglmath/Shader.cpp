
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Shader::Shader()
{
	programID = 0;
}

Shader::Shader(std::string vShaderPath, std::string fShaderPath)
{
	this->vShaderPath = vShaderPath;
	this->fShaderPath = fShaderPath;
	programID = getShaderProgram();

}

Shader::Shader(std::string vShaderPath, std::string fShaderPath, std::function<std::string(std::string)> fShaderPreprocessor)
{
	this->vShaderPath = vShaderPath;
	this->fShaderPath = fShaderPath;

	std::string vSource;
	std::string fSource;
	getShaderSourcesFromPaths(vSource, fSource, vShaderPath, fShaderPath);

	programID = getShaderProgramFromSources(vSource, fShaderPreprocessor(fSource));
}

unsigned int Shader::getShaderProgram()
{
	std::string vSource;
	std::string fSource;
	getShaderSourcesFromPaths(vSource, fSource, vShaderPath, fShaderPath);
	return getShaderProgramFromSources(vSource, fSource);
}

void Shader::getShaderSourcesFromPaths(std::string& vShaderSource, std::string& fShaderSource, std::string vShaderPath, std::string fShaderPath)
{
	std::ifstream vShaderStream;
	std::ifstream fShaderStream;

	vShaderStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fShaderStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		vShaderStream.open(vShaderPath);
		fShaderStream.open(fShaderPath);

		std::stringstream vShaderStringStream;
		std::stringstream fShaderStringStream;

		vShaderStringStream << vShaderStream.rdbuf();
		fShaderStringStream << fShaderStream.rdbuf();

		vShaderStream.close();
		fShaderStream.close();

		vShaderSource = vShaderStringStream.str();
		fShaderSource = fShaderStringStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

}

unsigned int Shader::getShaderProgramFromSources(std::string vShaderSource, std::string fShaderSource)
{
	const char* vSourceCstr = vShaderSource.c_str();
	const char* fSourceCstr = fShaderSource.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vSourceCstr, NULL);
	glShaderSource(fragmentShader, 1, &fSourceCstr, NULL);

	glCompileShader(vertexShader);
	logErrors(vertexShader, "SHADER");
	glCompileShader(fragmentShader);
	logErrors(fragmentShader, "SHADER");
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	logErrors(program, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Shader::use() const
{
	glUseProgram(programID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVec4(const std::string& name, float v0, float v1, float v2, float v3) const
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), v0, v1, v2, v3);
}

void Shader::setVec3(const std::string& name, float v0, float v1, float v2) const
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), v0, v1, v2);
}

void Shader::setVec2(const std::string& name, float v0, float v1) const
{
	glUniform2f(glGetUniformLocation(programID, name.c_str()), v0, v1);
}

void Shader::setBoolArray(const std::string& name, GLsizei size, bool* value) const
{
	glUniform1iv(glGetUniformLocation(programID, name.c_str()), size, (int*)value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::logErrors(unsigned int id, std::string type)
{
	char infoLog[1024];
	int success;
	if (type == "PROGRAM")
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			glGetProgramInfoLog(id, 1024, NULL, infoLog);
			std::cout << "PROGRAM::LINK::FAILED" << infoLog << std::endl;
		}
	}
	else if (type == "SHADER")
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(id, 1024, NULL, infoLog);
			std::cout << "SHADER::COMPILATION::FAILED" << infoLog << std::endl;
		}
	}
}