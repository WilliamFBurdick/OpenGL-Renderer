#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	GLuint ID;

	void Use();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

