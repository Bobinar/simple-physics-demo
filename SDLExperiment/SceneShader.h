#pragma once
#include <gl/glew.h>


namespace SceneShader
{
	GLbyte vertexShaderString[] =
		"#version 100\n"
		"precision highp float;\n"
		"uniform mat4 MVP;\n"
		"uniform mat4 M;\n"
		"uniform mat4 lightMVP;\n"
		"uniform vec3 worldLightPosition;\n "
		"attribute vec3 vPosition;    \n"
		"attribute vec3 vNormal;		\n"
		"varying vec3 worldNormal;	\n"
		"varying vec3 worldPosition;	\n"
		"varying vec4 lightSpaceProjectedCoords; \n"
		"void main()											\n"
		"{														\n"
		"   gl_Position = MVP * vec4(vPosition,1.0);			\n"
		"   worldPosition = (M * vec4(vPosition,1.0)).xyz;		\n"
		"   worldNormal = (M * vec4(vNormal,0.0)).xyz;			\n"
		"   lightSpaceProjectedCoords = lightMVP * vec4(vPosition, 1.0);  \n"
		"}														\n";

	GLbyte fragmentShaderString[] =
		"#version 100\n"
		"precision highp float;\n"
		"uniform vec3 worldLightPosition;"
		"varying vec3 worldNormal;\n"
		"varying vec3 worldPosition;	\n"
		"varying vec4 lightSpaceProjectedCoords; \n"
		"uniform sampler2D shadowMap; \n"
		"void main()                                  \n"
		"{                                            \n"
		"   vec3 distanceVector = worldLightPosition- worldPosition;\n"
		"   float distanceToLightSq= dot(distanceVector,distanceVector);\n"
		"   const float MaxSquaredLightDistance = 6.0;\n"
		"   float distanceAttenuation = clamp(1.0 - distanceToLightSq/MaxSquaredLightDistance , 0.0, 1.0);\n"
		"   float lightDot = dot(normalize(distanceVector), normalize(worldNormal))*0.5 + 0.5;  //Half N dot L     \n"
		"   vec4 normalizedLightSpaceProjectedCoords = (lightSpaceProjectedCoords / lightSpaceProjectedCoords.w)*0.5 +0.5;\n"
		"   const float Bias = -0.01;\n"
		"   float visibility = ((texture2D(shadowMap, normalizedLightSpaceProjectedCoords.xy).x) < (normalizedLightSpaceProjectedCoords.z + Bias)) ? 0.2 : 1.0; \n"
		"	gl_FragColor = clamp(distanceAttenuation * lightDot +0.15,0.0,1.0)  * vec4 (1.0, 1.0, 1.0, 1.0 ) * visibility;\n"
		"}                                            \n";
}
