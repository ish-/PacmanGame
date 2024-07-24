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
