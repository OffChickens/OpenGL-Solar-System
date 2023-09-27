#version 150

in vec4 vPosition;    // referenced in C++ program
in vec4 vNormal;
uniform mat4 mM;    // MVP matrices
uniform mat4 mV;
uniform mat4 mP;
out vec4 color;

void main () {
    color = normalize(vNormal);
    gl_Position = mP*mV*mM*vPosition;
}