#ifndef LIGHT
#define LIGHT
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shader.h"

class Light {
public:
    Light(GLfloat* position, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular);
    void set_shader_light_position(GLuint shader_ID);
    void set_light_position(GLfloat* newPosition);

    GLfloat*    position;
    GLfloat*    ambient;
    GLfloat*    diffuse;
    GLfloat*    specular;

private:
    GLuint        light_position_ID;        // The ID of the lightPosition variable in the current shader
};
#endif