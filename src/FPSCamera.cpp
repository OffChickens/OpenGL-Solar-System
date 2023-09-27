#include <iostream>
#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov/*degrees*/, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool is_flying = true)
{
    // The View matrix (the position and orientation of the camera)
    mV = glm::mat4( 1.0f ); // identity

    // The Perspective matrix - note that fov, aspect and the planes reside in the matrix.
    mP = glm::perspective( glm::radians( fov ), aspect, nearPlane, farPlane );

    mtx_skybox_view = glm::mat4( 1.0f );

    this->is_flying = is_flying;
    rotX = rotY = rotZ = 0;
    posX = posY = posZ = 0;
}

void FPSCamera::set_uniform_view_perspective(GLuint shader_ID)
{
    glUseProgram(shader_ID);

    // Find the location of the "mP" variable in the GPU vertex shader
    GLuint mpLoc = glGetUniformLocation(shader_ID, "mP");
    // Upload mP to the GPU shader
    glUniformMatrix4fv( mpLoc, 1, GL_FALSE, &mP[0][0] );

    // Upload mV to the GPU shader
    GLuint mvLoc = glGetUniformLocation(shader_ID, "mV");
    
    if ( !is_flying ) {
        // Create rotation matrix for X and Y rotations only (not Z)
        glm::mat4 rotXMatrix = glm::rotate(
            this->mV,
            rotX, //glm::degrees( rotX ),
            glm::vec3( 1.0f, 0.0f, 0.0f )
            );

        glUniformMatrix4fv( mvLoc, 1, GL_FALSE, &rotXMatrix[0][0] );
    }
    else {
        glUniformMatrix4fv( mvLoc, 1, GL_FALSE, &mV[0][0] );
    }
}

void FPSCamera::rotateX(GLfloat amount)
{
    if (is_flying) {
        glm::mat4 rot = glm::rotate(
            glm::mat4( 1.0f ),
            amount, //glm::degrees( amount ),
            glm::vec3( 1.0f, 0.0f, 0.0f )
        );
        //this->mV = this->mV * rot; // ***WRONG!
        this->mV = rot * this->mV; // correct
		rotX += amount; // radians
    }
}
void FPSCamera::rotateY(GLfloat amount)
{
    glm::mat4 rot = glm::rotate(
        glm::mat4( 1.0f ),
        amount, //glm::degrees( amount ),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );
    this->mV = rot * this->mV;
	rotY += amount; // radians
}

void FPSCamera::rotateZ(GLfloat amount)
{
    glm::mat4 rot = glm::rotate(
        glm::mat4( 1.0f ),
        amount, //glm::degrees( amount ),
        glm::vec3(0.0f, 0.0f, 1.0f)
        );
    this->mV = rot * this->mV;
	rotZ += amount; // radians
}

void FPSCamera::print_debug( char const* function_, GLfloat amount )
{
	if (std::abs(amount - 0.0001f) < 0.001f) return;
	std::cout
		<< '\r'
		<< "xyz == "
		<< posX
		<< " "
		<< posY
		<< " "
		<< posZ
		<< " rotXYZ == "
		<< rotX
		<< " "
		<< rotY
		<< " "
		<< rotZ
		;
	std::cout << " " << function_ << " " << amount << " units \n";
}
// Strafing -- moving the camera around
void FPSCamera::moveX(GLfloat amount)
{
    glm::mat4 trans = glm::translate(
        glm::mat4( 1.0f ),
        glm::vec3( amount, 0.0f, 0.0f )
        );
    this->mV = trans * this->mV;
    // see https://www.3dgep.com/understanding-the-view-matrix/
    // Converting between Camera Transformation Matrix and View Matrix
    glm::mat4 imV = glm::inverse( this->mV  );
    posX = imV[ 3 ].x;
    posY = imV[ 3 ].y;
    posZ = imV[ 3 ].z;
    //print_debug( __FUNCTION__, amount );
}
void FPSCamera::moveY(GLfloat amount)
{
    glm::mat4 trans = glm::translate(
        glm::mat4( 1.0f ),
        glm::vec3( 0.0f, amount, 0.0f )
        );
    this->mV = trans * this->mV;
    // see https://www.3dgep.com/understanding-the-view-matrix/
    // Converting between Camera Transformation Matrix and View Matrix
    glm::mat4 imV = glm::inverse( this->mV );
    posX = imV[ 3 ].x;
    posY = imV[ 3 ].y;
    posZ = imV[ 3 ].z;
    //print_debug( __FUNCTION__, amount );
}
void FPSCamera::moveZ(GLfloat amount)
{
    glm::mat4 trans = glm::translate(
        glm::mat4( 1.0f ),
        glm::vec3( 0.0f, 0.0f, amount )
        );
    this->mV = trans * this->mV;
    // see https://www.3dgep.com/understanding-the-view-matrix/
    // Converting between Camera Transformation Matrix and View Matrix
    glm::mat4 imV = glm::inverse( this->mV );
    posX = imV[ 3 ].x;
    posY = imV[ 3 ].y;
    posZ = imV[ 3 ].z;
    //print_debug( __FUNCTION__, amount );
}

void FPSCamera::set_perspective(GLfloat fov/*degrees*/, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
    mP = glm::perspective( glm::radians( fov ), aspect, nearPlane, farPlane );
}

/**Non-moving View-Projection*/
void FPSCamera::set_skybox_uniform_view_perspective(GLuint shader_ID)
{
    glUseProgram(shader_ID);

    // Find the location of the "mP" variable in the GPU vertex shader
    GLuint mpLoc = glGetUniformLocation(shader_ID, "mP");
    // Upload mP to the GPU shader
    glUniformMatrix4fv( mpLoc, 1, GL_FALSE, &mP[0][0] );

    // Upload mV to the GPU shader
    GLuint mvLoc = glGetUniformLocation(shader_ID, "mV");
    
    // 1) Use copy the mV matrix into mtx_skybox_view
    /*ANSWER*/
    mtx_skybox_view = this->mV;

    // 2) Set the last column mtx_skybox_view[3].xyz (except w)
    /*ANSWER*/
    //mtx_skybox_view[3][0] = 0; // works, but below syntax is preferred:
    //mtx_skybox_view[3][1] = 0;
    //mtx_skybox_view[3][2] = 0;
    mtx_skybox_view[3].x = 0; // this eliminates movement of the skybox
    mtx_skybox_view[3].y = 0;
    mtx_skybox_view[3].z = 0;

    if ( !is_flying ) {
        // Create stand-alone matrix for X and Y rotations only (not Z)
        glm::mat4 rotXMatrix = glm::rotate(
            mtx_skybox_view,
            rotX, //glm::degrees( rotX ),
            glm::vec3( 1.0f, 0.0f, 0.0f )
            );

        glUniformMatrix4fv( mvLoc, 1, GL_FALSE, &rotXMatrix[0][0] );
    }
    else {
        glUniformMatrix4fv( mvLoc, 1, GL_FALSE, &mtx_skybox_view[0][0] );
    }
}

void FPSCamera::set_spaceship_uniform_view_perspective(GLuint shader_ID)
{
    glUseProgram(shader_ID);

    // Find the location of the "mP" variable in the GPU vertex shader
    GLuint mpLoc = glGetUniformLocation(shader_ID, "mP");
    // Upload mP to the GPU shader
    glUniformMatrix4fv( mpLoc, 1, GL_FALSE, &mP[0][0] );

    // Upload mV to the GPU shader
    GLuint mvLoc = glGetUniformLocation(shader_ID, "mV");
    
    // 1) Use copy the mV matrix into mtx_skybox_view
    glm::mat4 mtx_car_view = glm::mat4( 1.0 );

	glUniformMatrix4fv( mvLoc, 1, GL_FALSE, &mtx_car_view[0][0] );
}

