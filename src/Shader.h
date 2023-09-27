#ifndef SHADER
#define SHADER
#include <cassert>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>

class Shader {
public:
    Shader(const char* vertexProgram, const char* fragmentProgram);
    ~Shader();
    GLint program_ID;
private:
    char* load_text_file(const char* filename);
    bool is_shader_compiled_okay (GLint shader_ID);
    bool is_linked_shader_okay (GLint program_ID);
    GLint compile_vertex_shader(char* shaderFile);
    GLint compile_fragment_shader(char* shaderFile);
    GLint link_shader_program(GLint vertex_shader_ID, GLint fragment_shader_ID);
};
#endif