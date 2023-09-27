#version 150

in vec3 fN;
in vec3 fE;
in vec3 fL;

out vec4 fColor;

in vec2 texCoord;                // UPDATE! This is the UV
uniform sampler2D texture;        // UPDATE! This is the texture

uniform vec4 ambient, diffuse, specular;
uniform float shininess;

void main() {
	vec3 N = normalize(-fN);    // the Normal
	vec3 L = normalize(fL);    // the Light direction
	vec3 E = normalize(-fE);    // the vertex position
	vec3 H = normalize(L + E); // the Half vector

	// Calculate the diffuse part - but don't go negative!
	float Kd = max(dot(N, L), 0.0);    // Intensity
	vec4 dif = Kd*texture2D(texture, texCoord);// * 2;                // ... and then color

	// Calculate the specular part. 
	float Ks = pow(max(dot(N, H), 0.0), shininess * 5);    // Intensity
	vec4 spec = Ks*specular;                            // ... and then color
	if (dot(N, L) < 0.0) {                                // But only if the light is in front!
		spec = vec4(0.0, 0.0, 0.0, 1.0);
	}

	// The final color is a combination of all of these
	fColor = ambient + dif + spec;
	//fColor = ambient*0.01 + dif*0.01 + spec + texture2D(texture, spec.xy);
	fColor.a = 1.0;
}