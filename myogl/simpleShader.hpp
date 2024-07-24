#pragma once
// #define SHADER_DEV

#include <string>
#include "util/loadShader.h"

#ifdef SHADER_DEV
#include "util/loadFile.h"
#else
std::string vertShaderCode = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 modelView;
	uniform mat4 modelViewProjection;
	uniform mat4 normalMat;
	uniform vec3 lightPos;

	out vec3 vNormal;
	out vec3 vLightVec;

	void main() {
		vec4 pos = modelViewProjection * vec4(aPos, 1.0);
		// normalMat_ = transpose(inverse(view * model));
		vNormal = normalize((normalMat * vec4(aNormal, 1.)).xyz);
		vLightVec = lightPos - pos.xyz;
		gl_Position = pos;
	}
	)";

std::string fragShaderCode = R"(
	#version 330 core

	uniform vec3 diffuseCol;

	in vec3 vNormal;
	in vec3 vLightVec;

	const float invRadiusSq = 0.00001;
	const vec3 ambientCol = vec3(.3, .13, .11);

	out vec4 fragColor;
	void main() {
		vec3 ambient = vec3(ambientCol * diffuseCol.xyz);
		float distSq = dot(vLightVec, vLightVec);
		float attenuation = clamp(1.0 - invRadiusSq * sqrt(distSq), 0.0, 1.0);
		attenuation *= attenuation;
		vec3 lightDir = vLightVec * inversesqrt(distSq);
		vec3 diffuse = max(dot(lightDir, vNormal), 0.0) * diffuseCol;

		vec3 finalColor = (ambient + diffuse) * attenuation;
		fragColor = vec4(finalColor, 1.);
	}
	)";
#endif

GLuint simpleShaderId = 0;
GLuint getSimpleShaderId()
{
#ifdef SHADER_DEV
  std::string vertShaderCode = loadFile("../shaders/pos-norm.vert.glsl");
  std::string fragShaderCode = loadFile("../shaders/const-norm.frag.glsl");
#endif

  if (!simpleShaderId)
    simpleShaderId = loadShader(vertShaderCode, fragShaderCode);
  return simpleShaderId;
}
