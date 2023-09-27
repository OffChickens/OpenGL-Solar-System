#ifndef SKYBOX
#define SKYBOX 
#include "Model.h"
#include "Shader.h"

class Skybox : public Model{
public:
    Skybox(Shader* shader);
    ~Skybox();
    void upload_CubeMap_images();
private:
    GLuint cube_map_ID;
};
#endif