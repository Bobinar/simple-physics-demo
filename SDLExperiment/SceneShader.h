#pragma once
#include <gl/glew.h>


namespace SceneShader
{
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
}
