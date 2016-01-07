#pragma once
#include <gl/glew.h>

namespace ShadowMapShader
{
	GLbyte vShaderStr[] =
		"#version 100\n"
		"precision highp float;\n"
		"uniform mat4 MVP;\n"
		"attribute vec3 vPosition;    \n"
		"void main()                  \n"
		"{                            \n"

		"   gl_Position = MVP * vec4(vPosition, 1);  \n"
		"}                            \n";

	GLbyte fShaderStr[] =
		"#version 100\n"
		"precision highp float;\n"
		"varying vec3 worldPosition;				  \n"
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4(1.0); // color result does not matter, we only need the depth buffer to be written correctly			  \n"
		"}                                            \n";
}
