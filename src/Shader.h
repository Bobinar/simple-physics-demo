#pragma once
#include <GL/glew.h>
#include "ShaderFactory.h"

class Shader
{

public:
	GLuint ProgramObject;

	Shader(GLbyte *vertexShaderString, GLbyte *fragmentShaderString)
	{
		ProgramObject = ShaderFactory::CompileShader(vertexShaderString, fragmentShaderString);
	}

	~Shader()
	{
		glDeleteProgram(ProgramObject);
	}
};
