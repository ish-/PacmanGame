#version 330 core

uniform vec3 diffuseCol;

out vec3 color;
void main() {
	color = diffuseCol;
}
