#pragma once

#include <GL/glew.h>

namespace ShaderFactory
{
	GLuint CompileShader(GLbyte *vertexShaderString, GLbyte *fragmentShaderString);
}