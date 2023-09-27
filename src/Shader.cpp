#include "Shader.h"

Shader::Shader(const char* vertexFilename, const char* fragmentFilename){
    // Read in the vertex program file
    char* vertexCode = load_text_file(vertexFilename);
    // Read in the fragment program file
    char* fragmentCode = load_text_file(fragmentFilename);
    // Check to make sure neither one is null.  If so, print an error and exit
    if ((vertexCode==NULL)||(fragmentCode==NULL)) {
        printf ("Can't find shader file\n");
        assert( !"Can't find shader file" );
        exit(-1);
    }
    // Make a vertex shader
    GLint vertexID = compile_vertex_shader(vertexCode);
    // Make a fragment shader
    GLint fragmentID = compile_fragment_shader(fragmentCode);
    // Make the shader program and assign it to the "program_ID" variable
    program_ID = link_shader_program(vertexID, fragmentID);
    printf ("Shader ID is %d\n", program_ID);
    // Clean up memory (the first two variables you created)
    delete[] (vertexCode);
    delete[] (fragmentCode);
}

char* Shader::load_text_file (const char* filename) {
    // Open the file
    FILE* fp = fopen(filename, "r");

    // Go to the beginning and get the length of the file
    fseek (fp, 0, SEEK_END);
    long file_length = ftell(fp);

    // Go back to the beginning and read the contents
    fseek (fp, 0, SEEK_SET);
    char* contents = new char[file_length+1];
    for (int i = 0; i < file_length+1; i++) {
        contents[i] = 0;
    }
    fread (contents, 1, file_length, fp);
    contents[file_length] = '\0';
    fclose(fp);

    return contents;
}

//Begin vertex and fragment shader functions
/// This function determines if a shader (either vertex or fragment) compiled correctly
bool Shader::is_shader_compiled_okay(GLint shader_ID){
    GLint compiled = 0;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &compiled);
    if (compiled) {
        return true;
    }
    else {
        GLint log_length;
        glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &log_length);
        char* msg_buffer = new char[log_length];
        glGetShaderInfoLog(shader_ID, log_length, NULL, msg_buffer);
        printf ("%s\n", msg_buffer);
        delete[] (msg_buffer);
        return false;
    }
}
// This function takes in the source code for a *vertex* shader,
// compiles it, and returns the ID (number) of the shader. It returns
// -1 if the shader didn't compile
GLint Shader::compile_vertex_shader (char* shader_source) {
    GLint vShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource (vShaderID, 1, (const GLchar**)&shader_source, NULL);
    glCompileShader(vShaderID);
    bool compiled_correctly = is_shader_compiled_okay(vShaderID);
    if (compiled_correctly) {
        return vShaderID;
    }
    return -1;
}
// This function takes in the source code for a *fragment* shader,
// compiles it, and returns the ID (number) of the shader. It returns
// -1 if the shader didn't compile
GLint Shader::compile_fragment_shader (char* shader_source) {
    GLint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource (fShaderID, 1, (const GLchar**)&shader_source, NULL);
    glCompileShader(fShaderID);
    bool compiled_correctly = is_shader_compiled_okay(fShaderID);
    if (compiled_correctly) {
        return fShaderID;
    }
    return -1;
}
//End vertex and fragment shader functions

//Begin shader program functions
// A helper function that takes in a program ID and returns whether
// or not it linked correctly.  If not, it prints out an error
// message letting you know what the problem was. This will
// be called from link_shader_program()
bool Shader::is_linked_shader_okay(GLint program_ID){
    GLint linked = 0;
    glGetProgramiv(program_ID, GL_LINK_STATUS, &linked);
    if (linked) {
        return true;
    }
    else {
        GLint log_length;
        glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &log_length);
        char* msg_buffer = new char[log_length];
        glGetProgramInfoLog(program_ID, log_length, NULL, msg_buffer);
        printf ("%s\n", msg_buffer);
        delete[] (msg_buffer);
        return false;
    }
}

// A function that takes in the IDs for vertex and fragment shader,
// and returns the ID of a shader program that has been linked.
GLint Shader::link_shader_program (GLint vertex_shader_ID, GLint fragment_shader_ID) {
    printf ("Linking %d with %d\n", vertex_shader_ID, fragment_shader_ID);
    GLint program_ID = glCreateProgram();
    glAttachShader(program_ID, vertex_shader_ID);
    glAttachShader(program_ID, fragment_shader_ID);
    glLinkProgram (program_ID);

    bool linked = is_linked_shader_okay (program_ID);

    if (linked) {
        return program_ID;
    }
    return -1;
}

Shader::~Shader(){
    glDeleteProgram(program_ID);
}
//End shader program functions
