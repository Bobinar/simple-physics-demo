#pragma once
#include <gl/glew.h>
#include "ShaderConstants.h"
#include <cassert>

namespace
{
	GLuint LoadShader(GLenum type, const char *shaderSrc)
	{
		GLuint shader;
		GLint compiled;

		// Create the shader object
		shader = glCreateShader(type);

		if (shader == 0)
			return 0;

		// Load the shader source
		glShaderSource(shader, 1, &shaderSrc, NULL);

		// Compile the shader
		glCompileShader(shader);

		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = new char[infoLen];

				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				SDL_Log("Error compiling shader:\n%s\n", infoLog);

				delete[] infoLog;
			}

			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
}

namespace ShaderFactory
{

	GLuint CompileShader(GLbyte *vertexShaderString, GLbyte *fragmentShaderString)
	{
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint programObject;
		GLint linked;

		vertexShader = LoadShader(GL_VERTEX_SHADER, (const char *)vertexShaderString);
		fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (const char *)fragmentShaderString);

		programObject = glCreateProgram();

		assert(programObject != 0);

		glAttachShader(programObject, vertexShader);
		glAttachShader(programObject, fragmentShader);

		glBindAttribLocation(programObject, ShaderConstants::PositionAttributeLocation, "vPosition");
		glBindAttribLocation(programObject, ShaderConstants::NormalAttributeLocation, "vNormal");

		glLinkProgram(programObject);

		glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			GLint infoLen = 0;

			glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = new char[infoLen];
				glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);

				SDL_Log("Error linking program:\n%s\n", infoLog);

				delete[] infoLog;
			}

			glDeleteProgram(programObject);
			assert(false);
		}

		return programObject;
	}

}