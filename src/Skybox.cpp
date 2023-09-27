#include "Skybox.h"

/*
#define BMP_NEG_X_FILE_PATH "../../Models/Lab8/BMP/NegX.bmp"
#define BMP_NEG_Y_FILE_PATH "../../Models/Lab8/BMP/NegY.bmp"
#define BMP_NEG_Z_FILE_PATH "../../Models/Lab8/BMP/NegZ.bmp"

#define BMP_POS_X_FILE_PATH "../../Models/Lab8/BMP/PosX.bmp"
#define BMP_POS_Y_FILE_PATH "../../Models/Lab8/BMP/PosY.bmp"
#define BMP_POS_Z_FILE_PATH "../../Models/Lab8/BMP/PosZ.bmp"

#define BMP_NEG_X_FILE_PATH "../../Models/Lab8/skybox/xneg.bmp"
#define BMP_POS_X_FILE_PATH "../../Models/Lab8/skybox/xpos.bmp"
#define BMP_NEG_Y_FILE_PATH "../../Models/Lab8/skybox/yneg.bmp"
#define BMP_POS_Y_FILE_PATH "../../Models/Lab8/skybox/ypos.bmp"
#define BMP_NEG_Z_FILE_PATH "../../Models/Lab8/skybox/zneg.bmp"
#define BMP_POS_Z_FILE_PATH "../../Models/Lab8/skybox/zpos.bmp"

#define BMP_NEG_X_FILE_PATH "../../Models/Lab8/skybox_compass/xneg.bmp"
#define BMP_POS_X_FILE_PATH "../../Models/Lab8/skybox_compass/xpos.bmp"
#define BMP_NEG_Y_FILE_PATH "../../Models/Lab8/skybox_compass/yneg.bmp"
#define BMP_POS_Y_FILE_PATH "../../Models/Lab8/skybox_compass/ypos.bmp"
#define BMP_NEG_Z_FILE_PATH "../../Models/Lab8/skybox_compass/zneg.bmp"
#define BMP_POS_Z_FILE_PATH "../../Models/Lab8/skybox_compass/zpos.bmp"

#define BMP_NEG_X_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_lf.bmp"
#define BMP_POS_X_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_rt.bmp"
#define BMP_NEG_Y_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_dn.bmp"
#define BMP_POS_Y_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_up.bmp"
#define BMP_NEG_Z_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_ft.bmp"
#define BMP_POS_Z_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_bk.bmp"

#define BMP_POS_Y_FILE_PATH "../../Models/solar_system/spacebox/spacebox_bottom.bmp"
#define BMP_POS_X_FILE_PATH "../../Models/solar_system/spacebox/spacebox_east.bmp"
#define BMP_POS_Z_FILE_PATH "../../Models/solar_system/spacebox/spacebox_north.bmp"
#define BMP_NEG_Z_FILE_PATH "../../Models/solar_system/spacebox/spacebox_south.bmp"
#define BMP_NEG_Y_FILE_PATH "../../MODELS/solar_system/spacebox/spacebox_top.bmp"
#define BMP_NEG_X_FILE_PATH "../../Models/solar_system/spacebox/spacebox_west.bmp"
*/

#define BMP_NEG_X_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_lf.bmp"
#define BMP_POS_X_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_rt.bmp"
#define BMP_NEG_Y_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_dn.bmp"
#define BMP_POS_Y_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_up.bmp"
#define BMP_NEG_Z_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_ft.bmp"
#define BMP_POS_Z_FILE_PATH "../../Models/Lab8/GalaxyMap/Galaxy_bk.bmp"

Skybox::Skybox(Shader* shader) : Model(shader) {
    // Just call the parent constructor
}

Skybox::~Skybox() {
}

void Skybox::upload_CubeMap_images() {
    unsigned char* temp_image_data;
    int width, height, size;

    // 1) Enable cube mapping
    /*ANSWER*/
    glEnable (GL_TEXTURE_CUBE_MAP);
    
    // 2) Generate a texture ID to hold the cube map (use tex_buffer_ID.back(), which is inherited from Model)
    /*ANSWER*/
    tex_buffer_ID.push_back( 0 );
    glGenTextures (1, &tex_buffer_ID.back());        //Note: we inherited this from Model
    
    // 3) Bind tex_buffer_ID.back() as the current buffer, but make sure you do it as a cube map!
    /*ANSWER*/
    glBindTexture (GL_TEXTURE_CUBE_MAP, tex_buffer_ID.back());
    
    // 4) Load all six images into the correct "place" in the cube map. Look in the Resources folders for the names.
    //    Make sure you delete the memory you used between each call.
    /*ANSWER*/
    load_bmp_file( BMP_NEG_X_FILE_PATH, &width, &height, &size, &temp_image_data );
    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, temp_image_data);
    delete (temp_image_data);
    load_bmp_file( BMP_NEG_Y_FILE_PATH, &width, &height, &size, &temp_image_data );
    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, temp_image_data);
    delete (temp_image_data);
    load_bmp_file( BMP_NEG_Z_FILE_PATH, &width, &height, &size, &temp_image_data );
    glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, temp_image_data);
    delete (temp_image_data);
    load_bmp_file( BMP_POS_X_FILE_PATH, &width, &height, &size, &temp_image_data );
    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, temp_image_data);
    delete (temp_image_data);
    load_bmp_file( BMP_POS_Y_FILE_PATH, &width, &height, &size, &temp_image_data );
    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, temp_image_data);
    delete (temp_image_data);
    load_bmp_file( BMP_POS_Z_FILE_PATH, &width, &height, &size, &temp_image_data );
    glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, temp_image_data);
    delete (temp_image_data);

    // 5) Set the S, T and R parameters for the texture, but make sure you use GL_TEXTURE_CUBE_MAP
    /*ANSWER*/
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // 6) Get the current ID/location of the "cubeMap" variable in the shader and put it into "cube_map_ID"
    /*ANSWER*/
    cube_map_ID = glGetUniformLocation(shader->program_ID, "cubeMap");
    printf ("location for cubeMap: %d\n", cube_map_ID);

    // Bind the current buffer to texture unit 0.  This makes the cubeMap variable refer to this texture.
    glUniform1i(cube_map_ID, 0);
}

