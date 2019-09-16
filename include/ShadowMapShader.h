#pragma once
#include <GL/glew.h>

namespace ShadowMapShader
{
	GLbyte vertexShaderString[] =
#ifdef __APPLE__
		"#version 110\n"
#else
		"#version 100\n"
		"precision highp float;\n"
#endif
		"uniform mat4 MVP;\n"
		"attribute vec3 vPosition;    \n"
		"attribute vec3 vNormal;		\n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = MVP * vec4(vPosition, 1);  \n"
		"}                            \n";

	GLbyte fragmentShaderString[] =
#ifdef __APPLE__
		"#version 110\n"
#else
		"#version 100\n"
		"precision highp float;\n"
#endif
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4(1.0); // color result does not matter, we only need the depth buffer to be written correctly			  \n"
		"}                                            \n";
}
