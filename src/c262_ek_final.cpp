#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Shader.h"
#include "Model.h"
#include "FPSCamera.h"

#include "Light.h"
#include "Skybox.h"


// c262_lab11_solution.cpp
// shader path configuration:
#define    BCC_PROJECT_NAME "c262_ek_final"
#define    BCC_ROOT_PATH "../../labs/"

#define    BCC_VERTEX_TEXTURE_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/TextureShader_v.c"
#define    BCC_FRAGMENT_TEXTURE_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/TextureShader_f.c"

#define    BCC_VERTEX_SOLID_COLOR_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/SolidShader_v.c"
#define    BCC_FRAGMENT_SOLID_COLOR_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/SolidShader_f.c"

#define    BCC_VERTEX_SKYBOX_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/SkyboxShader_v.c"
#define    BCC_FRAGMENT_SKYBOX_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/SkyboxShader_f.c"

#define    BCC_FRAGMENT_TEXTURE_SUN BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/SunShader_f.c"

//#define BMP_OBJ_FILE_PATH "../../Models/Lab8/BMP/scales.bmp"
//#define BMP_GRASS_FILE_PATH "../../Models/Lab8/BMP/grass.bmp"
#define BMP_SUN_PATH "../../Models/Lab8/PlanetTextures/sunmap.bmp"
#define BMP_EARTH_PATH "../../Models/Lab8/PlanetTextures/earthmap.bmp"
// TODO: add BMP path to mars texture here: "../../Models/Lab8/PlanetTextures/marsmap1k.bmp"
#define BMP_MARS_PATH "../../Models/Lab8/PlanetTextures/marsmap1k.bmp"

#define BMP_JUPITER_PATH "../../Models/Lab8/PlanetTextures/jupiter2_1k.bmp"
#define BMP_MERCURY_PATH "../../Models/Lab8/PlanetTextures/mercurymap.bmp"
#define BMP_VENUS_PATH "../../Models/Lab8/PlanetTextures/venusmap.bmp"
#define BMP_SATURN_PATH "../../Models/Lab8/PlanetTextures/saturnmap.bmp"
#define BMP_NEPTUNE_PATH "../../Models/Lab8/PlanetTextures/neptunemap.bmp"
#define BMP_URANUS_PATH "../../Models/Lab8/PlanetTextures/uranusmap.bmp"

#define BMP_MOON_PATH "../../Models/Lab8/PlanetTextures/moonmap1k.bmp"
//#define BMP_MOON_PATH "../../Models/Lab8/PlanetTextures/earthmap.bmp"

//------------------------------------------------------------------
// HOW TO TEST DIFFERENT MODELS:
//------------------------------------------------------------------
// Include one of the models defined in a header file,
// -or-
// load te model from the OBJ file by defining MODEL_OBJ_FILENAME
//------------------------------------------------------------------

// OBJ Models that contain textture coordinates:


//#define MODEL_OBJ_FILENAME "../../Models/Spheretree/bunny-1500.obj"
//#define MODEL_OBJ_FILENAME "../../Models/Spheretree/cow-1500.obj"
//#define MODEL_OBJ_FILENAME "../../Models/Spheretree/dragon-1500.obj"  // scale_amount = 2.5f;
//#define MODEL_OBJ_FILENAME "../../Models/Spheretree/lamp600.obj"        // scale_amount = 0.007f
//#define MODEL_OBJ_FILENAME "../../Models/Spheretree/shammy.obj"          // scale_amount = 0.001f
//#define MODEL_OBJ_FILENAME "../../Models/Spheretree/teapot.obj"      // scale_amount = 0.001f

//#define MODEL_OBJ_FILENAME "../../Models/Misc/cube5.obj" // a bigger cube (5 units) -- scale_amount = 0.15f;
//#define MODEL_OBJ_FILENAME "../../Models/Misc/cube_blender.obj.txt"
//#define MODEL_OBJ_FILENAME "../../Models/Misc/plane.obj"
//#define MODEL_OBJ_FILENAME "../../Models/Misc/car.obj" // scale_amount = 0.15f;
//#define MODEL_OBJ_FILENAME "../../Models/Misc/cube_textured.obj" // scale_amount = 0.5f;
//#define MODEL_OBJ_FILENAME "../../Models/Misc/triangle.obj" // scale_amount = 0.5f;
//#define MODEL_OBJ_FILENAME "../../Models/Misc/square.obj" // scale_amount = 0.5f;
//#define MODEL_OBJ_FILENAME "../../Models/Lab8/OBJ/dragon.obj"  // old
//#define MODEL_PLANE_FILENAME "../../Models/Lab8/OBJ/plane.obj" // old
#define MODEL_CUBE_FILENAME "../../Models/Lab8/OBJ/cube.obj"
#define MODEL_SPACESHIP_FILENAME "../../Models/Misc/car.obj"
//#define MODEL_OBJ_FILENAME MODEL_SPACESHIP_FILENAME
#define MODEL_OBJ_FILENAME "../../Models/Lab8/OBJ/sphere.obj"    // new
#define MODEL_RING_FILENAME "../../Models/Lab8/OBJ/saturnring.obj"    // new

Model* sun_model;
Model* earth_model;
Model* moon_model;
Model* mars_model;
Model* jupiter_model;
Model* mercury_model;
Model* venus_model;
Model* saturn_model;
Model* neptune_model;
Model* uranus_model;
Model* ring_model;
// TODO: add pointer to mars mpdel here:

Model* spaceship_model;

Skybox* skybox;

FPSCamera* camera;
GLfloat theta;
Light*    light;                    

GLfloat yVel, zVel, xVel, max_speed, wing_roll;


//============= GLUT data and functions =================
GLfloat fov;
GLfloat fov_increment = 3.0f;
GLfloat fov_min = 5.0f; // don't allow fov less than 5 degrees
GLfloat fov_max = 60.0f; // don't allow fov more than 60 degrees
GLfloat aspect = 1.0f;
enum {
    ZOOM_MIN,
    ZOOM_MAX,
    ZOOM_VARIABLE
};
int zooming = ZOOM_MIN;
GLboolean steering = false;

void zoom_in();
void zoom_out();
GLfloat prev_mouse_X;
GLfloat prev_mouse_Y;
bool is_first_time = true;

GLuint fps_rate = 72; // frames per second

//========================================================

void animate()
{
	static GLint count_frames = 0; // as time goes by, keep counting frames
	static const time_t start_time = time( nullptr ); // captured only once
	static GLint prior_second = 0; // remembers seconds since program start

	++count_frames;
	time_t current_time = time(nullptr);
	// calculate how many seconds have elapsed since program started
	GLint seconds_elapsed = GLint( current_time - start_time );
	if (seconds_elapsed != prior_second) {
		// one second has elapsed
		fps_rate = count_frames; // update fps rate
		count_frames = 0; // reset count for the next second
	}
	prior_second = seconds_elapsed;

	//std::cout << "\r fps_rate: " << fps_rate << "\r"; // DEBUG
	glutPostRedisplay();
}

void render() {
	// keep rotation speed 360 degrees per 30 seconds:
	theta += glm::radians(360.0f/*degrees*/ / (30 /*seconds*/ * fps_rate));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if (zooming) zoom_in();
    //else if (fov <= fov_max) zoom_out();
    if (zooming == ZOOM_MAX) zoom_in();
    else if (zooming == ZOOM_VARIABLE);
    else if (fov <= fov_max) zoom_out();
    camera->moveZ (zVel);
    camera->moveX (xVel);
    camera->moveY (yVel);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //-------------------------------------------------
    // Do everything related to the Sun
    //-------------------------------------------------

    // (3) rotate the Sun a little bit
    sun_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta / 10, //glm::degrees(theta/10),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // (0) scale the Sun
    sun_model->mM = glm::scale(  // Scale second
        sun_model->mM,
        glm::vec3(3.0f, 3.0f, 3.0f)
        );

    // Compute the matrix for rotation of normals:
    //sun_model->mR = glm::transpose(glm::inverse(sun_model->mM));

    // Sets the cameras VP matrices for this shader 
    camera->set_uniform_view_perspective(sun_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(sun_model->shader->program_ID);
    sun_model->render();

    //-------------------------------------------------
    // Do everything related to the Earth
    //-------------------------------------------------

    // (3) rotate around Sun
    earth_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta, //glm::degrees(theta),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // (2) move from the Sun 10 units
    earth_model->mM = glm::translate(
        earth_model->mM,
        glm::vec3( 0.0f, 0.0f, -12.0f ) // move object 10 units back
        );

    // (1) Rotate Earth on its axis
    earth_model->mM = glm::rotate(
        earth_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    earth_model->mM = glm::scale(  // Scale second
        earth_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.3f, 0.3f, 0.3f)
    );

	// to determine location of the object,
	// extract its position in space as follows:
	//GLfloat EposX = earth_model->mM[3].x;
	//GLfloat EposY = earth_model->mM[3].y;
	//GLfloat EposZ = earth_model->mM[3].z;

	//std::cout << "X " << int( EposX ) << " Y " << int( EposY ) << " Z " << int( EposZ ) << "       \r"; // DEBUG

    // Compute the matrix for rotation of normals:
    //earth_model->mR = glm::transpose( glm::inverse( earth_model->mM ));

    // Sets the cameras VP matrices for this shader 
    camera->set_uniform_view_perspective(earth_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(earth_model->shader->program_ID);
    earth_model->render();


	//-------------------------------------------------
	// TODO Do everything related to Mars
	//-------------------------------------------------

	// TODO (3) rotate around Sun
    mars_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 53/100,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

	// TODO (2) move from the Sun 15 units
    mars_model->mM = glm::translate(
        mars_model->mM,
        glm::vec3(0.0f, 0.0f, -15.0f) // move object 15 units back
    );

	// TODO (1) Rotate Mars on its axis
    mars_model->mM = glm::rotate(
        mars_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Mercury
    mars_model->mM = glm::scale(  // Scale second
        mars_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.2f, 0.2f, 0.2f)
    );
    
    camera->set_uniform_view_perspective(mars_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(mars_model->shader->program_ID);
    mars_model->render();

	// to determine location of the object,
	// extract its position in space. as follows:
	//GLfloat MposX = mars_model->mM[3].x;
	//GLfloat MposY = mars_model->mM[3].y;
	//GLfloat MposZ = mars_model->mM[3].z;

	// compute distance between two objects in space:
	//glm::vec3 vE{ EposX, EposY, EposZ };
	//glm::vec3 vM{ MposX, MposY, MposZ };
	//vM = -vM;
	//GLfloat dist = glm::length(vE + vM);
	//std::cout << "dist " << dist << "          \r";

    //-------------------------------------------------
    // TODO Do everything related to Mercury
    //-------------------------------------------------

    // TODO (3) rotate around Sun
    mercury_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 4,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 7.5 units
    mercury_model->mM = glm::translate(
        mercury_model->mM,
        glm::vec3(0.0f, 0.0f, -7.5f) // move object 7.5 units back
    );

    // TODO (1) Rotate Mercury on its axis
    mercury_model->mM = glm::rotate(
        mercury_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Mercury
    mercury_model->mM = glm::scale(  // Scale second
        mercury_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.1f, 0.1f, 0.1f)
    );

    camera->set_uniform_view_perspective(mercury_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(mercury_model->shader->program_ID);
    mercury_model->render();

    //-------------------------------------------------
    // TODO Do everything related to Venus
    //-------------------------------------------------

    // TODO (3) rotate around Sun
    venus_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 8/5,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 9 units
    venus_model->mM = glm::translate(
        venus_model->mM,
        glm::vec3(0.0f, 0.0f, -9.0f) // move object 9 units back
    );

    // TODO (1) Rotate Venus on its axis
    venus_model->mM = glm::rotate(
        venus_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Venus
    venus_model->mM = glm::scale(  // Scale second
        venus_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.3f, 0.3f, 0.3f)
    );

    camera->set_uniform_view_perspective(venus_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(venus_model->shader->program_ID);
    venus_model->render();
    
    //-------------------------------------------------
    // TODO Do everything related to Jupiter
    //-------------------------------------------------

    // TODO (3) rotate around Sun
    jupiter_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 9/100,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 19 units
    jupiter_model->mM = glm::translate(
        jupiter_model->mM,
        glm::vec3(0.0f, 0.0f, -19.0f) // move object 19 units back
    );

    // TODO (1) Rotate Jupiter on its axis
    jupiter_model->mM = glm::rotate(
        jupiter_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Jupiter
    jupiter_model->mM = glm::scale(  // Scale second
        jupiter_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.9f, 0.9f, 0.9f)
    );

    camera->set_uniform_view_perspective(jupiter_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(jupiter_model->shader->program_ID);
    jupiter_model->render();

    //-------------------------------------------------
    // TODO Do everything related to Saturn
    //-------------------------------------------------

    // TODO (3) rotate around Sun
    saturn_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 7/200,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 23 units
    saturn_model->mM = glm::translate(
        saturn_model->mM,
        glm::vec3(0.0f, 0.0f, -23.0f) // move object 23 units back
    );

    // TODO (1) Rotate Saturn on its axis
    saturn_model->mM = glm::rotate(
        saturn_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Saturn
    saturn_model->mM = glm::scale(  // Scale second
        saturn_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.8f, 0.8f, 0.8f)
    );

    camera->set_uniform_view_perspective(saturn_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(saturn_model->shader->program_ID);
    saturn_model->render();

    //-------------------------------------------------
// TODO Do everything related to Saturn Ring
//-------------------------------------------------

// TODO (3) rotate around Sun
    ring_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 7/200,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 23 units
    ring_model->mM = glm::translate(
        ring_model->mM,
        glm::vec3(0.0f, 0.0f, -23.0f) // move object 23 units back
    );


    // TODO (1) Rotate Saturn on its axis
    ring_model->mM = glm::rotate(
        ring_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(10.0f, 2.0f, 0.0f)
    );

    // TODO (0) scale the Saturn ring
    ring_model->mM = glm::scale(  // Scale second
        ring_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.07f, 0.07f, 0.07f)
    );

    camera->set_uniform_view_perspective(ring_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(ring_model->shader->program_ID);
    ring_model->render();

    //-------------------------------------------------
    // TODO Do everything related to Uranus
    //-------------------------------------------------

    // TODO (3) rotate around Sun
    uranus_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 1/80,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 27 units
    uranus_model->mM = glm::translate(
        uranus_model->mM,
        glm::vec3(0.0f, 0.0f, -27.0f) // move object 27 units back
    );

    // TODO (1) Rotate Uranus on its axis
    uranus_model->mM = glm::rotate(
        uranus_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Uranus
    uranus_model->mM = glm::scale(  // Scale second
        uranus_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.7f, 0.7f, 0.7f)
    );

    camera->set_uniform_view_perspective(uranus_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(uranus_model->shader->program_ID);
    uranus_model->render();

    //-------------------------------------------------
    // TODO Do everything related to Neptune
    //-------------------------------------------------

    // TODO (3) rotate around Sun
    neptune_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta * 7/1000,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (2) move from the Sun 31 units
    neptune_model->mM = glm::translate(
        neptune_model->mM,
        glm::vec3(0.0f, 0.0f, -31.0f) // move object 31 units back
    );

    // TODO (1) Rotate Neptune on its axis
    neptune_model->mM = glm::rotate(
        neptune_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // TODO (0) scale Neptune
    neptune_model->mM = glm::scale(  // Scale second
        neptune_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.69f, 0.69f, 0.69f)
    );

    camera->set_uniform_view_perspective(neptune_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(neptune_model->shader->program_ID);
    neptune_model->render();

    //-----------------------------------------------------------
    // Do everything related to the skybox. Note, we want to see the *inside* of the box,
    //-----------------------------------------------------------
    // so we flip the normals by winding Clock-Wise (GL_CW)
    // 1) Change the winding of the triangles
    /*ANSWER*/
    glFrontFace (GL_CW);
    // 2) Scale the mM variable of the skybox to make it 100 times bigger
    /*ANSWER*/
    // original skybox OBJ has 0.5f side.
    skybox->mM = glm::scale(
        glm::mat4( 1.0f ),
        glm::vec3( 100.0f, 100.0f, 100.0f ) // make cube of 50 units
        );
    // 3) Call the camera's set_skybox_uniform_view_perspective() function
    /*ANSWER*/
    camera->set_skybox_uniform_view_perspective(skybox->shader->program_ID);
    // 4) Render the skybox
    /*ANSWER*/
    skybox->render();
    // 5) Reset the winding order
    /*ANSWER*/
    glFrontFace (GL_CCW);
    
    //--------------------------------------------------------
    // Do everything related to the Moon
    //--------------------------------------------------------
    // (5) rotate around Sun
    moon_model->mM = glm::rotate(
        glm::mat4(1.0f),
        theta, //glm::degrees(theta),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // (4) move from the Sun 10 units (same orbit as the Earth)
    moon_model->mM = glm::translate(
        moon_model->mM,
        glm::vec3(0.0f, 0.0f, -12.0f) // move object 10 units back
        );

    // (3) rotate the Moon around Earth
    moon_model->mM = glm::rotate(
        moon_model->mM,
        theta * 10, //glm::degrees(theta),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // (2) move the Moon to the orbit around the Earth
    moon_model->mM = glm::translate(
        moon_model->mM,
        glm::vec3(2.0f, 0.0f, 0.0f)
        );
    // (1) Rotate Moon on around the axis
    moon_model->mM = glm::rotate(
        moon_model->mM,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // (0) scale the Moon
    moon_model->mM = glm::scale(  // Scale second
        moon_model->mM,
        //glm::vec3( 100.0f, 100.0f, 100.0f )
        glm::vec3(0.1f, 0.1f, 0.1f)
        );

    // Compute the matrix for rotation of normals:
    //moon_model->mR = glm::transpose( glm::inverse( moon_model->mM ));

    camera->set_uniform_view_perspective(moon_model->shader->program_ID);
    // Sets the light position for the object model's shader:
    light->set_shader_light_position(moon_model->shader->program_ID);
    moon_model->render();

    //---------------------------------------------------------
    // Do everything related to the space ship cockpit
    //---------------------------------------------------------
    // Note: you can probably *store* space ship matrices to improve efficiency - since it's not moving.

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 mtx_translate = glm::translate(
        glm::mat4(1.0f),
        glm::vec3( 0.0f, -0.1f, -0.1f ) // move object 20 units back
        );
    glm::mat4 mtx_scale = glm::scale(  // Scale first
        glm::mat4(1.0f),
        glm::vec3( 0.05f, 0.05f, 0.05f )
        );
    glm::mat4 mtx_rotate = glm::rotate(
        glm::mat4(1.0f),
        glm::radians( 90.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) // move object 20 units back
        );

    spaceship_model->mM = mtx_translate * mtx_scale * mtx_rotate;

    camera->set_spaceship_uniform_view_perspective(spaceship_model->shader->program_ID);
    glFrontFace (GL_CW);
    light->set_shader_light_position(spaceship_model->shader->program_ID);
    //spaceship_model->render();
    glFrontFace (GL_CCW);

    glutSwapBuffers();
}

// Any time the window is resized, this function gets called. Set by the
// "glutReshapeFunc" in main.
void change_viewport(int w, int h){
    aspect = GLfloat( w ) / h;
    glViewport(0, 0, w, h);
}

void zoom_in() {
    // FOV == field of view of the camera
    // zooming is set by the callback_mouse_button()
    fov -= fov_increment;
    if ( fov < fov_min ) fov = fov_min; // don't allow fov less than min
    camera->set_perspective( fov, aspect, 0.1f, 1000.0f );
}

void zoom_out() {
    fov += fov_increment;
    if ( fov > fov_max ) fov = fov_max; // don't allow fov greater than max
    camera->set_perspective( fov, aspect, 0.1f, 1000.0f );
}

//Begin GLUT callback functions
// This one is the keyboard func (clearly) which moves the camera using WASD
// Note: 033 is an "octal" number and is the same thing as Escape, so you could call "exit" on that
void keyboard_down (unsigned char key, int x, int y) {
    switch (key) {
        case 033: // octal equivalent of the Escape key
            glutLeaveMainLoop();
            break;
        case 'w': zVel = max_speed; break;
        case 's': zVel = -max_speed; break;
        case 'a': xVel = max_speed; break;
        case 'd': xVel = -max_speed; break;

        case 'Y': yVel = max_speed; break;
        case 'y': yVel = -max_speed; break;

        case 'z': camera->rotateZ(-wing_roll); break;
        case 'x': camera->rotateZ(wing_roll); break;
    }
}

void keyboard_up (unsigned char key, int x, int y) {
    switch (key) {
        case 033:
            glutLeaveMainLoop();
            break;
        case 'w': zVel=0; break;
        case 's': zVel=0; break;
        case 'a': xVel=0; break;
        case 'd': xVel=0; break;
        case 'Y': yVel=0; break;
        case 'y': yVel=0; break;
    }
}

// The mouse button callback.  If the user right-clicks and holds, zoom in.
void callback_mouse_button(int button, int state, int x, int y) {
    // Note: mouse wheel event reports like a button click, but
    //       twice with GLUT_DOWN then GLUT_UP
    //printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
    //printf ("%d, %d\n", button, state);
    if ( button == 3  ) {
        // Wheel event scroll up
        //printf("3-Scroll %s At %f\n", (state == GLUT_UP) ? "Up" : "Down", fov);
        if ( state == GLUT_UP ) {
            zooming = ZOOM_VARIABLE;
            zoom_in();
        }

    } else if ( button == 4 ) {
        // Wheel event scroll down
        //printf("4-Scroll %s At %f\n", (state == GLUT_UP) ? "Up" : "Down", fov);
        if ( state == GLUT_UP ) {
            zooming = ZOOM_VARIABLE;
            zoom_out();
        }
    } else if (state == GLUT_UP) {
        // Disregard redundant GLUT_UP events
        if (button == GLUT_RIGHT_BUTTON) zooming = ZOOM_MIN;
        steering = false;

    } else if (button == GLUT_RIGHT_BUTTON) {
        // GLUT_UP or GLUT_DOWN
        zooming = ZOOM_MAX;

    } else if (button == GLUT_LEFT_BUTTON) {
        steering = true;

    }
}

// The mouse motion callback.  Compare the current mouse position with the old
// one to figure out which way to rotate the camera.
void look (int x, int y) {
    if ( !steering ) return;
    if (is_first_time) {
        //is_first_time = false;
        // Check for zooming mode, but do not allow the model
        // jump off the screen when the mouse enters the window from an outside
        // region of screen. Thus, to activate the mouse, you need to click the
        // mouse to zoom the view AND move it a little:
        is_first_time = false;
        prev_mouse_X = GLfloat( x );
        prev_mouse_Y = GLfloat( y );
        return;
    }
    GLfloat deltaX = prev_mouse_X-x;
    GLfloat deltaY = prev_mouse_Y-y;

    if ( deltaX < -6.0f ) { deltaX = -1.0f; }
    if ( deltaX >  6.0f ) { deltaX =  1.0f; }
    if ( deltaY < -6.0f ) { deltaY = -1.0f; }
    if ( deltaY >  6.0f ) { deltaY =  1.0f; }

    //printf ("DX=%f, DY=%f\n", deltaX, deltaY);

    if (zooming) {
        deltaX /= 10.0f;
        deltaY /= 10.0f;
    }

    if (camera->is_flying) {
        camera->rotateX((-deltaY)/( 50 * 5 ));
        camera->rotateY((-deltaX)/( 20 * 1 ));
        camera->rotateZ((-deltaX)/( 70 * 5 ));
    }
    else {
        camera->rotateX( deltaY / 150.0f );
        camera->rotateY( deltaX / 150.0f );

    }
    
    prev_mouse_X = GLfloat( x );
    prev_mouse_Y = GLfloat( y );
}
//End GLUT callback functions

int main (int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up some memory buffers for our display
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
    // Set the window size
    glutInitWindowSize(800, 800);
    // Create the window with the title
    glutCreateWindow( "Solar System Lab" );
    // Bind the two functions (above) to respond when necessary
    glutReshapeFunc(change_viewport);
    glutDisplayFunc(render);
    glutIdleFunc( animate );
    glutMouseFunc (callback_mouse_button);
    glutKeyboardFunc(keyboard_down);
    glutKeyboardUpFunc(keyboard_up);
    glutPassiveMotionFunc(look);
    glutMotionFunc(look);
    
    // Remember, we always init GLEW after we create a window
    // This asks the GPU driver for all of those gl functions
    if (GLEW_OK != glewInit()) {
        assert( !"glewInit() failed -- exiting" );
        exit(1);
    }

    glEnable( GL_MULTISAMPLE );
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable (GL_DEPTH_TEST);
    
    glEnable (GL_CULL_FACE);
    // These two commands aren't necessary, these are the default settings
    glCullFace(GL_BACK);
    glFrontFace (GL_CCW);

    Shader* texture_shader = new Shader( BCC_VERTEX_TEXTURE_SHADER, BCC_FRAGMENT_TEXTURE_SHADER );

    Shader* solid_color_shader = new Shader( BCC_VERTEX_SOLID_COLOR_SHADER, BCC_FRAGMENT_SOLID_COLOR_SHADER );

    Shader* skybox_shader = new Shader( BCC_VERTEX_SKYBOX_SHADER, BCC_FRAGMENT_SKYBOX_SHADER );
    
	Shader* sun_shader = new Shader(BCC_VERTEX_TEXTURE_SHADER, BCC_FRAGMENT_TEXTURE_SUN);

    // Directional light parameters:
    GLfloat light_pos[] = {0.0f, 0.0f, 0.0f, 1.0f/*0.0f==Direction light, 1.0f otherwise*/};        
    GLfloat light_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat light_diffuse[] = {0.7f, 0.7f, 0.9f, 1.0f};
    GLfloat light_specular[] = {0.2f, 0.2f, 0.2f, 1.0f};

    GLfloat material_ambient[] = {0.1f, 0.1f, 0.2f, 1.0f};
    GLfloat material_diffuse[] = {0.2f, 0.2f, 0.7f, 1.0f};
    GLfloat material_specular[] = {0.1f, 0.1f, 1.0f, 1.0f};
    //GLfloat shininess = 100.0f;
    GLfloat shininess = 10.0f;

    light = new Light(light_pos, light_ambient, light_diffuse, light_specular);
    //========================================================
    
    //sun_model = new Model(texture_shader); // TODO: use sun_shader
    sun_model = new Model(sun_shader);
    sun_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    sun_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    sun_model->set_light(light);
    //sun_model->upload_bitmap_texture( BMP_OBJ_FILE_PATH );  // old
    sun_model->upload_bitmap_texture(BMP_SUN_PATH, "texture");       // new
    //sun_model->upload_bitmap_texture(BMP_MOON_PATH, "texture_bump");       // new
    sun_model->upload_2_server();

    earth_model = new Model(texture_shader);
    earth_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    earth_model->set_material(material_ambient, material_diffuse, material_specular, shininess);        
    earth_model->set_light(light);
    //earth_model->upload_bitmap_texture( BMP_OBJ_FILE_PATH );  // old
    earth_model->upload_bitmap_texture( BMP_EARTH_PATH, "texture" );       // new
    //earth_model->upload_bitmap_texture( BMP_MOON_PATH, "texture_bump" );       // new
    earth_model->upload_2_server();

	// TODO: set up Mars model and upload its texture
    mars_model = new Model(texture_shader);
    mars_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    mars_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    mars_model->set_light(light);
    mars_model->upload_bitmap_texture(BMP_MARS_PATH, "texture");       // new
    mars_model->upload_2_server();

    // TODO: set up Jupiter model and upload its texture
    jupiter_model = new Model(texture_shader);
    jupiter_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    jupiter_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    jupiter_model->set_light(light);
    jupiter_model->upload_bitmap_texture(BMP_JUPITER_PATH, "texture");       // new
    jupiter_model->upload_2_server();

    // TODO: set up Mercury model and upload its texture
    mercury_model = new Model(texture_shader);
    mercury_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    mercury_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    mercury_model->set_light(light);
    mercury_model->upload_bitmap_texture(BMP_MERCURY_PATH, "texture");       // new
    mercury_model->upload_2_server();

    // TODO: set up Venus model and upload its texture
    venus_model = new Model(texture_shader);
    venus_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    venus_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    venus_model->set_light(light);
    venus_model->upload_bitmap_texture(BMP_VENUS_PATH, "texture");       // new
    venus_model->upload_2_server();

    // TODO: set up Uranus model and upload its texture
    uranus_model = new Model(texture_shader);
    uranus_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    uranus_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    uranus_model->set_light(light);
    uranus_model->upload_bitmap_texture(BMP_URANUS_PATH, "texture");       // new
    uranus_model->upload_2_server();

    // TODO: set up Saturn model and upload its texture
    saturn_model = new Model(texture_shader);
    saturn_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    saturn_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    saturn_model->set_light(light);
    saturn_model->upload_bitmap_texture(BMP_SATURN_PATH, "texture");       // new
    saturn_model->upload_2_server();

    // TODO: set up Saturn model and upload its texture
    ring_model = new Model(texture_shader);
    ring_model->load_OBJ_file(MODEL_RING_FILENAME);
    ring_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    ring_model->set_light(light);
    ring_model->upload_bitmap_texture(BMP_SATURN_PATH, "texture");       // new
    ring_model->upload_2_server();

    // TODO: set up Neptune model and upload its texture
    neptune_model = new Model(texture_shader);
    neptune_model->load_OBJ_file(MODEL_OBJ_FILENAME);
    neptune_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    neptune_model->set_light(light);
    neptune_model->upload_bitmap_texture(BMP_NEPTUNE_PATH, "texture");       // new
    neptune_model->upload_2_server();

    moon_model = new Model(texture_shader);
    ///moon_model->load_OBJ_file( MODEL_PLANE_FILENAME );  // old
    moon_model->load_OBJ_file( MODEL_OBJ_FILENAME );       // new
    moon_model->set_material(material_ambient, material_diffuse, material_specular, shininess);
    //moon_model->upload_bitmap_texture( BMP_GRASS_FILE_PATH ); // old
    moon_model->upload_bitmap_texture( BMP_MOON_PATH, "texture" );         // new
    moon_model->set_light(light);
    moon_model->upload_2_server();

    skybox = new Skybox(skybox_shader);
    skybox->load_OBJ_file( MODEL_CUBE_FILENAME );
    skybox->upload_CubeMap_images();
    skybox->upload_2_server();

    spaceship_model = new Model(solid_color_shader); // for wireframe
    spaceship_model->load_OBJ_file( MODEL_SPACESHIP_FILENAME );
    spaceship_model->upload_2_server();

    // Set up the camera 
    fov = fov_max;
    // Camera pozition is at the origin looking in -z direction:
    //camera = new FPSCamera(fov, aspect, 0.1f, 1000.0f, false/*is_flying*/ );
    camera = new FPSCamera(fov, aspect, 0.1f, 1000.0f, true );
    camera->moveY( -3.0f );
    camera->moveZ( -4.0f );
    
    zooming = false;
    prev_mouse_X = 400;
    yVel = zVel = xVel = 0.0f;
    max_speed = 0.006f;
    wing_roll = 0.03f; // in radians
    theta = 0.0f;

    glutMainLoop();

    return 0;
}
