#version 150

out vec4 vFragColor;
uniform samplerCube cubeMap;
in vec3 texCoords;

void main (void) {
	vec4 color = texture(cubeMap, texCoords);
	float alpha = color.a;
	vFragColor = 3 * color; // increase intensity 3 times
	vFragColor.a = color.a;
}
