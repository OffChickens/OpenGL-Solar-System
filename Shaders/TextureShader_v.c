#version 150

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;            // UPDATE! - coming in from our OpenGL program
out vec2 texCoord;            // UPDATE! - going out to the fragment shader

uniform mat4 mM;            // M_odel matrix of model
uniform mat4 mV;            // V_iew matrix of camera
uniform mat4 mP;            // P_erspective matrix of camera
uniform mat4 mR;            // R_otation of model just for rotating normals

uniform vec4 lightPosition;    

out vec3 fN;                // Normal of vertex            
out vec3 fE;                // Vertex in Eye (camera) space
out vec3 fL;                // Light position

void main () {

	//transpose inverse matrix
	mat4 tiM = transpose(inverse(mM));
    fN = (mR*tiM*vNormal).xyz;
    fE = (mV*mM*vPosition).xyz;
    fL = (lightPosition).xyz;

    // If it's not a direction light (e.g. the sun), then L depends on the position of the vertex
    if (lightPosition.w != 0.0) {
        fL = lightPosition.xyz - (mM*vPosition).xyz;
    }

    gl_Position = mP*mV*mM*vPosition;
    texCoord = vTexCoord;    // UPDATE! Notice, all we do is pass this through to the fragment shader
}