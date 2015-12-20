#pragma once
#include <gl/glew.h>
#include <SDL2/SDL.h>
#include <cassert>

class SampleShader
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

	SampleShader()
	{
		// dot(M * vec4(vColor,0), vec4(worldLightDirection,0) * -1.0);\n"
		//			"   float lightDot = dot(M * vec4(vColor,0), vec4(worldLightDirection,0) * -1.0);\n"
		//"   vec3 worldNormal = (M * vec4(vColor,0.0)).xyz;"
		//"   float lightDot = dot(M * vec4(vColor,0), vec4(worldLightDirection,0));\n"
		//"   vec3 toLight = normalize(worldLightPosition - worldPosition);"
		//	"   float lightDot = dot(M * vec4(vColor,0), vec4(toLight,0));\n"
		//"   float lightDot = dot(M * vec4(vColor,0), vec4(worldLightDirection,0));\n"
		GLbyte vShaderStr[] =
			"#version 100\n"
			"precision highp float;\n"
			"uniform mat4 MVP;\n"
			"uniform mat4 M;\n"
			"uniform vec3 worldLightDirection;"
			"uniform vec3 worldLightPosition;"
			"attribute vec3 vPosition;    \n"
			"attribute vec3 vColor;		\n"
			"varying vec3 worldNormal;	\n"
			"varying vec3 worldPosition;	\n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = MVP * vec4(vPosition,1.0);  \n"
			"   worldPosition = (M * vec4(vPosition,1.0)).xyz;\n"
			"   worldNormal = (M * vec4(vColor,0.0)).xyz;			\n"
			"}                            \n";
		
		//"   float distanceToLight= length(worldLightPosition - worldPosition);\n"
		GLbyte fShaderStr[] =
			"#version 100\n"
			"precision highp float;\n"
			"uniform vec3 worldLightPosition;"
			"varying vec3 worldNormal;\n"
			"varying vec3 worldPosition;	\n"
			"void main()                                  \n"
			"{                                            \n"
			"   vec3 distanceVector = worldLightPosition- worldPosition;"
			"   float distanceToLight= dot(distanceVector,distanceVector);\n"
			"   float distanceAttenuation = clamp(1.0 - distanceToLight/6.0 , 0.0, 1.0);\n"
			"   float lightDot = dot(normalize(distanceVector), normalize(worldNormal))/2.0 + 0.5;\n"
			"  gl_FragColor = clamp(distanceAttenuation * lightDot +0.15,0.0,1.0)  * vec4 (1.0, 1.0, 1.0, 1.0 );\n"
			"}                                            \n";

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

	~SampleShader()
	{
		glDeleteProgram(ProgramObject);
	}
};
