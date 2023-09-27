#include "Light.h"

Light::Light(GLfloat* position, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular)
{
    this->position = new GLfloat[4];
    this->ambient = new GLfloat[4];
    this->diffuse = new GLfloat[4];
    this->specular = new GLfloat[4];

    // Save the data in our attributes
    for (int i = 0; i < 4; i++) {
        this->position[i] = position[i];
        this->ambient[i] = ambient[i];
        this->diffuse[i] = diffuse[i];
        this->specular[i] = specular[i];
    }
}

void Light::set_shader_light_position(GLuint shader_ID)
{
    glUseProgram(shader_ID);
    light_position_ID = glGetUniformLocation(shader_ID, "lightPosition");
    glUniform4fv(light_position_ID, 1, position); 
}

void Light::set_light_position(GLfloat* newPosition)
{
    for (int i = 0; i < 4; i++) {
        this->position[i] = newPosition[i];
    }
}
