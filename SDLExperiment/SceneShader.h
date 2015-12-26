#pragma once
#include <gl/glew.h>


namespace SceneShader
{
	GLbyte vShaderStr[] =
		"#version 100\n"
		"precision highp float;\n"
		"uniform mat4 MVP;\n"
		"uniform mat4 M;\n"
		"uniform mat4 lightMVP;\n"
		"uniform vec3 worldLightPosition;\n "
		"attribute vec3 vPosition;    \n"
		"attribute vec3 vColor;		\n"
		"varying vec3 worldNormal;	\n"
		"varying vec3 worldPosition;	\n"
		"varying vec4 lightSpaceProjectedCoords; \n"
		"void main()											\n"
		"{														\n"
		"   gl_Position = MVP * vec4(vPosition,1.0);			\n"
		"   worldPosition = (M * vec4(vPosition,1.0)).xyz;		\n"
		"   worldNormal = (M * vec4(vColor,0.0)).xyz;			\n"
		"   lightSpaceProjectedCoords = lightMVP * vec4(vPosition, 1.0);  \n"
		"}														\n";

	GLbyte fShaderStr[] =
		"#version 100\n"
		"precision highp float;\n"
		"uniform vec3 worldLightPosition;"
		"varying vec3 worldNormal;\n"
		"varying vec3 worldPosition;	\n"
		"varying vec4 lightSpaceProjectedCoords; \n"
		"uniform sampler2D shadowMap; \n"
		"void main()                                  \n"
		"{                                            \n"
		"   vec3 distanceVector = worldLightPosition- worldPosition;"
		"   float distanceToLightSq= dot(distanceVector,distanceVector);\n"
		"   float distanceAttenuation = clamp(1.0 - distanceToLightSq/6.0 , 0.0, 1.0);\n"
		"   float lightDot = dot(normalize(distanceVector), normalize(worldNormal))/2.0 + 0.5;\n"
		"   vec4 normalizedLightSpaceProjectedCoords = (lightSpaceProjectedCoords / lightSpaceProjectedCoords.w)*0.5 +0.5;"
		"   float visibility = (texture2D(shadowMap, normalizedLightSpaceProjectedCoords.xy).x < normalizedLightSpaceProjectedCoords.z) ? 0.1 : 1.0; \n"
		"	gl_FragColor = clamp(distanceAttenuation * lightDot +0.15,0.0,1.0)  * vec4 (1.0, 1.0, 1.0, 1.0 ) * visibility;\n"
		"}                                            \n";
}
