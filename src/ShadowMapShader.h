#pragma once
#include <GL/glew.h>

namespace ShadowMapShader
{
	GLbyte vertexShaderString[] =
		"#version 100\n"
		"precision highp float;\n"
		"uniform mat4 MVP;\n"
		"attribute vec3 vPosition;    \n"
		"attribute vec3 vNormal;		\n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = MVP * vec4(vPosition, 1);  \n"
		"}                            \n";

	GLbyte fragmentShaderString[] =
		"#version 100\n"
		"precision highp float;\n"
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4(1.0); // color result does not matter, we only need the depth buffer to be written correctly			  \n"
		"}                                            \n";
}
