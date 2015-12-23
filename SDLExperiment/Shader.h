#pragma once
#include <gl/glew.h>
#include <SDL2/SDL.h>
#include <cassert>

class Shader
{
private:
	static GLuint LoadShader(GLenum type, const char *shaderSrc)
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

public:
	GLuint ProgramObject;

	Shader(GLbyte *vShaderStr, GLbyte *fShaderStr)
	{

		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint programObject;
		GLint linked;

		// Load the vertex/fragment shaders
		vertexShader = LoadShader(GL_VERTEX_SHADER, (const char *)vShaderStr);
		fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (const char *)fShaderStr);

		// Create the program object
		programObject = glCreateProgram();

		assert(programObject != 0);
			

		glAttachShader(programObject, vertexShader);
		glAttachShader(programObject, fragmentShader);

		// Bind vPosition to attribute 0   
		glBindAttribLocation(programObject, 0, "vPosition");
		glBindAttribLocation(programObject, 1, "vColor");

		// Link the program
		glLinkProgram(programObject);

		// Check the link status
		glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			GLint infoLen = 0;

			glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				//char* infoLog = (char *)malloc(sizeof(char) * infoLen);
				char* infoLog = new char[infoLen];
				glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);

				SDL_Log("Error linking program:\n%s\n", infoLog);

				delete[] infoLog;
			}

			glDeleteProgram(programObject);
			assert(false);
		}

		// Store the program object
		ProgramObject = programObject;
	}

	~Shader()
	{
		glDeleteProgram(ProgramObject);
	}
};
