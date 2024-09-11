#include "Renderer.h"
#include <cstdio>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		printf("[OpenGL Error] (%u): %s %s : %i\n", error, function, file, line);
		return false;
	}
	return true;
}