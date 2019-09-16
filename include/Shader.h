#pragma once

#include <ShaderFactory.h>

#include <GL/glew.h>

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
