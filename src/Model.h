#ifndef MODEL
#define MODEL

#include <vector>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Light.h"

#include "../../../common/Wavefront.h"

class Model {
public:
    Model(Shader* shader);        // Note, in C++, constructors/destructors aren't inherited
    virtual ~Model();
    void set_geometry(GLfloat vertices[], int size);
    void set_normals(GLfloat* normals, int size);
    void set_vertex_colors(GLfloat* colors, int size);
    void set_texture_coordinates(GLfloat* uvs, int size);
    void load_OBJ_file( const char* filename );
    void set_index_buffer(GLuint* indices, int size);    
    void set_material (GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess);
    void load_bmp_file(const char* filename, int* width, int* height, int* size, unsigned char** pixel_data);
    virtual void upload_bitmap_texture (const char* filename, const char* sampler_name);            
    virtual void set_light (Light* light);
    
    void upload_2_server();
    virtual void render();

    glm::mat4    mR;            // A rotation matrix (used in the shader to rotate normals)

    glm::mat4    mM;            // A matrix holding all orientation information for this model.
                            // This will get passed off the mV variable in the shader
                            // This is the model part of the MVP matrix
    Shader*        shader;        // The shader object for this model
    Light*        light;        // The *only* light for this object
    
protected:
    
    GLfloat*    vertices;    // x, y, z
    GLfloat*    normals;    // Vertex normals
    GLfloat*    uvs;        // Texture coordinates
    GLfloat*    colors;        // Colors for each vertex
    GLuint*        indices;    // Index buffer                            
    GLuint        total_vertices, total_normals, total_UVs, total_colors, total_indices;
    GLuint        vao;        // Vertex Array Object
    GLuint        vbo;        // Vertex Buffer Object
    GLuint        index_vbo;    // Index Buffer Object
    //GLuint        tex_buffer_ID;        // A texture buffer ID            // UPDATE!
    std::vector<GLuint> tex_buffer_ID;        // A texture buffer ID            // UPDATE!

    GLfloat*    ambient, *combined_ambient;    // Ambient color                
    GLfloat*    diffuse, *combined_diffuse;    // Diffuse color                
    GLfloat*    specular, *combined_specular;    // Specular color                
    GLfloat        shininess;    // Shininess                    

    GLuint        vPosition;    // A handle for the variable "vPosition" in the shader
    GLuint        vNormal;    // A handle for the variable "vNormal" in the shader
    GLuint        vTexCoord;    // A handle for the variable "vTexCoord" in the shader
    GLuint        texture;    // A handle for the variable "texture" in the fragment shader
    std::vector<GLuint> texture_sampler_ID;        // A handle for the variable "texture" in the fragment shader
    GLuint        vColor;        // A handle for the variable "vColor" in the shader
    GLuint        umM;        // A handle for the variable "mM" in the shader
    GLuint        umR;        // A handle for the variable "mR" in the shader
    GLuint        ambientID;    // A handle for the ambient variable        
    GLuint        diffuseID;    // A handle for the diffuse variable        
    GLuint        specularID;    // A handle for the specular variable    
    GLuint        shineID;    // A handle for the shininess variable
};
#endif