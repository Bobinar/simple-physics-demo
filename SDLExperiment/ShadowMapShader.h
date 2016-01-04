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

	//packFloatToVec4i function comes from: http://www.ozone3d.net/blogs/lab/20080604/glsl-float-to-rgba8-encoder/
	GLbyte fShaderStr[] =
		"#version 100\n"
		"precision highp float;\n"
		"vec4 packFloatToVec4i(const float value)\n"
		"{\n"
		"const vec4 bitSh = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);\n"
		"const vec4 bitMsk = vec4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);\n"
		"vec4 res = fract(value * bitSh);\n"
		"res -= res.xxyz * bitMsk;\n"
		"return res;\n"
		"}\n"
		"varying vec3 worldPosition;				  \n"
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = packFloatToVec4i(gl_FragCoord.z);			  \n"
		"}                                            \n";
}
