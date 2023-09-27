#ifndef CAMERA
#define CAMERA
#include <GL/glew.h>
#include <GL/freeglut.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::perspective
// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>

class FPSCamera {
public:
    FPSCamera(GLfloat fov/*degrees*/, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool is_flying);

    // Transform functions.  All take in an amount of rotation or translation and can be positive or negative.
    void rotateX(GLfloat amount);    // Look left/right
    void rotateY(GLfloat amount);    // Look up/down
    void rotateZ(GLfloat amount);    // Bank left/right (like an airplane).  This isn't commonly used in FPS.
    void moveX(GLfloat amount);        // Strafing left/right
    void moveY(GLfloat amount);        // Floating up/down or jumping
    void moveZ(GLfloat amount);        // Forward/backward

    void set_uniform_view_perspective(GLuint shader_ID); // This function sets the View and Perspective in the shader
    void set_skybox_uniform_view_perspective(GLuint shader_ID); // UPDATE! This function is used for the skybox
	void set_spaceship_uniform_view_perspective(GLuint shader_ID);
    void set_perspective(GLfloat fov/*degrees*/, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);

    bool is_flying;

    glm::mat4 mP;    // the P_erspective matrix.  This is usually set once.
    glm::mat4 mV;    // the V_iew matrix.  This usually changes every frame.

private:
    // These are only used if the camera isn't flying (standard FPS)
    GLfloat rotX, rotY, rotZ; // angles between world and camera coordinate systems
    GLfloat posX, posY, posZ; // position of the camera in the world
	void print_debug(char const* function_, GLfloat amount);

private:
    glm::mat4 mtx_skybox_view;    // skybox view matrix, changes every frame
};

#endif