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
