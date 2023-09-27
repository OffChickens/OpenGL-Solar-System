#include "Model.h"

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model(Shader* shader) {
    this->shader = shader;
    glGenVertexArrays(1, &vao);                            // Generate a VAO
    glGenBuffers(1, &vbo);                                // Generate a buffer object
    index_vbo = 0;                                        // Will be generated if indices are loaded
    vertices = normals = uvs = colors = NULL;            // Assume no vertex information
    total_vertices = total_normals = total_UVs = total_colors = 0;    // Set all sizes to 0
    ambient = diffuse = specular = NULL;                
    shininess = 0.0f;                                    
    vPosition = vNormal = vTexCoord = vColor = umM = -1;
    ambientID = diffuseID = specularID = shineID = -1;    

    mM = glm::mat4(1.0f);                                // By default, the Model matrix is an identity matrix.
    mR = glm::mat4(1.0f);

    indices = NULL;
    total_indices = 0;

    umM = glGetUniformLocation(shader->program_ID, "mM");// Find the mM variable in the shader 
    umR = glGetUniformLocation(shader->program_ID, "mR");
    printf ("mM variable in shader is %d, and mR is %d\n", umM, umR);
}
Model::~Model() {
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vNormal);
    glDisableVertexAttribArray(vTexCoord);
    glDisableVertexAttribArray(vColor);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    //delete (shader);
    delete (vertices);
    delete (normals);
    delete (indices);
    delete (uvs);
    delete (colors);
    delete (ambient);
    delete (diffuse);
    delete (specular);
}

//Begin data upload functions
void Model::set_geometry(GLfloat* vertices, int size){
    printf ("Copying %d vertices\n", size/sizeof(GLfloat)/4);
    this->vertices = vertices;
    this->total_vertices = size/sizeof(GLfloat)/4;
    vPosition = glGetAttribLocation(shader->program_ID, "vPosition");
    printf ("***vPosition ID is %d for shader_ID %d\n", vPosition, shader->program_ID);
}
void Model::set_normals(GLfloat* normals, int size){
    printf ("Copying %d normals....\n", size/sizeof(GLfloat)/3);
    this->normals = normals;
    this->total_normals = size/sizeof(GLfloat)/3;
    vNormal = glGetAttribLocation(shader->program_ID, "vNormal");
    printf ("vNormal ID is: %d\n", vNormal);
}
void Model::set_vertex_colors(GLfloat* colors, int size){
    printf ("Copying %d colors...\n", size/sizeof(GLfloat)/4);
    this->colors = colors;
    this->total_colors = size/sizeof(GLfloat)/4;
    vColor = glGetAttribLocation(shader->program_ID, "vColor");
    printf ("vColor ID is: %d\n", vColor);
}
void Model::set_texture_coordinates(GLfloat* uvs, int size){
    printf ("Copying %d uvs...\n", size/sizeof(GLfloat)/2);
    this->uvs = uvs;
    this->total_UVs = size/sizeof(GLfloat)/2;
    vTexCoord = glGetAttribLocation(shader->program_ID, "vTexCoord");
    printf ("vTexCoord ID is: %d\n", vTexCoord);
}

void Model::set_index_buffer(GLuint* indices, int size){                    
    printf ("Copying indices...\n");
    this->indices = indices;
    this->total_indices = size/sizeof(GLuint);
    glBindVertexArray(vao);
    glGenBuffers(1, &index_vbo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    // This time, we'll just go ahead and upload the data to the buffer because
    // we're not packing multiple data sets into the buffer
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    printf ("Loaded %d indices in index_vbo %d\n", this->total_indices, index_vbo);
}
// Set the material of this model
void Model::set_material (GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess)
{
    if (this->ambient==NULL) { 
        this->ambient = new GLfloat[4];
        this->combined_ambient = new GLfloat[4];    // Combination of material and light
        ambientID = glGetUniformLocation(shader->program_ID, "ambient");
        printf ("Ambient ID is %d\n", ambientID);
    }
    if (this->diffuse==NULL) {
        this->diffuse = new GLfloat[4];
        this->combined_diffuse = new GLfloat[4];    // Combination of material and light
        diffuseID = glGetUniformLocation(shader->program_ID, "diffuse");
        printf ("Diffuse ID is %d\n", diffuseID);
    }
    if (this->specular==NULL) {
        this->specular = new GLfloat[4];
        this->combined_specular = new GLfloat[4];    // Combination of material and light
        specularID = glGetUniformLocation(shader->program_ID, "specular");
        printf ("Specular ID is %d\n", specularID);
    }

    for (int i = 0; i < 4; i++) {
        this->ambient[i] = ambient[i];
        this->diffuse[i] = diffuse[i];
        this->specular[i] = specular[i];
    }
    this->shininess = shininess;
    shineID = glGetUniformLocation(shader->program_ID, "shininess");
}

// Header info found at http://atlc.sourceforge.net/bmp.html
void Model::load_bmp_file(const char* filename, int* width, int* height, int* size, unsigned char** pixel_data) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf( "Couldn't open file %s aborting\n", filename );
        assert( !filename );
        exit( 1 );
    }
    short identifier = -1;
    fread(&identifier, 1, sizeof(short), fp); printf ("Identifer is: %c\n", identifier);
    int filesize = -1;
    fread(&filesize, 1, sizeof(int), fp); printf ("filesize is: %d\n", filesize);
    int reserved = -1;
    fread(&reserved, 1, sizeof(int), fp); printf ("reserved is: %d\n", reserved);
    int bitmap_offset = -1;
    fread(&bitmap_offset, 1, sizeof(int), fp); printf ("bitmap_offset is: %d\n", bitmap_offset);
    int bitmap_header_size = -1;
    fread(&bitmap_header_size, 1, sizeof(int), fp); printf ("bitmap_header_size is: %d\n", bitmap_header_size);
    int bitmap_width = -1;
    fread(&bitmap_width, 1, sizeof(int), fp); printf ("bitmap_width is: %d\n", bitmap_width);
    int bitmap_height = -1;
    fread(&bitmap_height, 1, sizeof(int), fp); printf ("bitmap_height is: %d\n", bitmap_height);
    short bitmap_planes = -1;
    fread(&bitmap_planes, 1, sizeof(short), fp); printf ("bitmap_planes is: %d\n", bitmap_planes);
    short bits_per_pixel= -1;
    fread(&bits_per_pixel, 1, sizeof(short), fp); printf ("bits_per_pixel is: %d\n", bits_per_pixel);
    int compression = -1;
    fread(&compression, 1, sizeof(int), fp); printf ("compression is: %d\n", compression);
    int bitmap_data_size = -1;
    fread(&bitmap_data_size, 1, sizeof(int), fp); printf ("bitmap_data_size is: %d\n", bitmap_data_size);
    int hresolution = -1;
    fread(&hresolution, 1, sizeof(int), fp); printf ("hresolution is: %d\n", hresolution);
    int vresolution = -1;
    fread(&vresolution, 1, sizeof(int), fp); printf ("vresolution is: %d\n", vresolution);
    int num_colors = -1;
    fread(&num_colors, 1, sizeof(int), fp); printf ("num_colors is: %d\n", num_colors);
    int num_important_colors = -1;
    fread(&num_important_colors, 1, sizeof(int), fp); printf ("num_important_colors is: %d\n", num_important_colors);
    
    // Jump to the data already!
    fseek (fp, bitmap_offset, SEEK_SET);
    unsigned char* data = new unsigned char[bitmap_data_size];
    // Read data in BGR format
    fread (data, sizeof(unsigned char), bitmap_data_size, fp);
    
    // Make pixel_data point to the pixels
    *pixel_data = data;
    *size = bitmap_data_size;
    *width = bitmap_width;
    *height = bitmap_height;
    fclose(fp);
}

void Model::upload_bitmap_texture(const char* filename, const char* sampler_name) {
    // Load the bitmap from file and upload to GPU
    int width = -1;
    int height = -1;
    int size = -1;
    GLubyte* bitmap_data;

    load_bmp_file(filename, &width, &height, &size, &bitmap_data);

    // Note: before you begin, remember that the upload_2_server function uploads the UVs onto the GPU

    tex_buffer_ID.push_back( 0 );
    glGenTextures(1, &tex_buffer_ID.back() );
    glBindTexture(GL_TEXTURE_2D, tex_buffer_ID.back() );
    // Load the bitmap onto the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(tex_buffer_ID.back());

    // Get the ID of the variable "texture" from the fragment shader
    texture_sampler_ID.push_back( glGetUniformLocation(shader->program_ID, sampler_name) );
    printf ("Texture variable in shader is %d\n", texture_sampler_ID.back());
}

void Model::set_light(Light* light) {
    this->light = light;
}
//End data upload functions

// Pre-condition: You must have called at least one of the functions above
// This function loads all of the data onto the GPU.
void Model::upload_2_server() {
    // Bind the vao
    glBindVertexArray(vao);
    // Bind vbo as the current VBO. Note: the VBO was created in the constructor
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    int offset = 0;
    
    printf ("Loading data onto the GPU buffer %d...\n", vbo);
    // Calculate the size of the buffer we need
    int sizeBuffer = (total_vertices*4+total_normals*3+total_UVs*2+total_colors*4)*sizeof(GLfloat);
    
    // Tell OpenGL that this data probably won't change and is used for drawing (GL_STATIC_DRAW).
    // Do not upload any data yet (NULL), but we specify the size (sizeBuffer). We'll pass data 
    // later using glBufferSubData
    
    // Call glBufferData and tell the GPU how big the buffer is.  Do NOT load any data yet.
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, NULL, GL_STATIC_DRAW);

    // If the vertices aren't NULL, load them onto the GPU. Offset is currently 0.
    if (vertices) {
        printf ("Loading vertices onto GPU...\n");
        // Load the vertices into the GPU buffer. Note the size is the number of vertices * 3(x, y, z)
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_vertices*4*sizeof(GLfloat), this->vertices);
        // You should read this as "vPosition should read in 3 GL_FLOATs that aren't normalized (GL_FALSE).
        // The data has a stride of 0 (non-interlaced) and starts with an offset of 0 in the buffer."
        glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
        offset+=total_vertices*4*sizeof(GLfloat);
    }
    // Load in the vertex normals right after the vertex coordinates.  Remember, there are 3 values for a normal
    if (normals) {
        printf ("Loading normals onto GPU...\n");
        
        // Load in the vertex data onto the GPU using glBufferSub. Put it right after the vertices
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_normals*3*sizeof(GLfloat), this->normals);
        // Note that the normal data has an offset that starts after the vertex data in the buffer
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
        offset+=total_normals*3*sizeof(GLfloat);
        
    }
    // Load in the texture coordinates right after the normals. Remember, there are 2 values for each UV
    if (uvs) {
        printf ("Loading UVs onto GPU...\n");
        
        // Call glBufferSubData to put the UVs onto the GPU. There are only 2 components in each UV
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_UVs*2*sizeof(GLfloat), this->uvs);
        // Call glVertexAttribPointer to tell vTexCoord where to find it's data. 
        glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
        offset+=total_UVs*2*sizeof(GLfloat);
    }
    // Load in the color coordinates right after the texture coordinates. There are 4 values for a color
    if (colors) {
        printf ("Loading colors onto GPU...\n");
        
        // Put the color info onto the GPU. CAREFUL! There are 4 parts to each color
        glBufferSubData(GL_ARRAY_BUFFER, offset, total_colors*4*sizeof(GLfloat), this->colors);
        // Specify where the color attributes (vColor) are located
        glVertexAttribPointer (vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(offset));
        offset+=total_colors*4*sizeof(GLfloat);
    }
}

void Model::render(){
    glBindVertexArray(vao);                        // Activate the VAO again. All that buffer info is remembered
    glUseProgram(shader->program_ID);            // Use the shader program
    if (vertices) {
        glEnableVertexAttribArray(vPosition);    // It's a pain, but we always have to enable shader variables
    }
    if (normals) {
        glEnableVertexAttribArray(vNormal);
    }
    if (vTexCoord != -1) {
        glEnableVertexAttribArray(vTexCoord);
        // Turn on 2D texturing
        glEnable(GL_TEXTURE_2D);
        for ( unsigned int idx = 0; idx < tex_buffer_ID.size(); ++idx ) {
            // Set it to be texture 0
            glActiveTexture(GL_TEXTURE0+idx);
            // Bind the texture we loaded as the current texture
            glBindTexture(GL_TEXTURE_2D, tex_buffer_ID[idx]);
            // and upload to the "texture" variable in the fragment shader
            glUniform1i(texture_sampler_ID[idx], idx);
        }
    }
    if (vColor != -1) {
        glEnableVertexAttribArray(vColor);
    }
    if (umM != -1) {
        // Upload Model matrix to the uniform
        glUniformMatrix4fv( umM, 1, GL_FALSE, &mM[0][0] );
    }
    if (umR != -1) {
        glUniformMatrix4fv( umR, 1, GL_FALSE, &mR[0][0] );
    }
    if (ambient) {
        // If light is specified,
        // First, calculate the color using the material AND the light
        for (int i = 0; i < 4; i++) {
            combined_ambient[i] = ambient[i]*light->ambient[i];
            combined_diffuse[i] = diffuse[i]*light->diffuse[i];
            combined_specular[i] = specular[i]*light->specular[i];
        }
        // Upload to the shader
        glUniform4fv(ambientID, 1, combined_ambient);
        glUniform4fv(diffuseID, 1, combined_diffuse);
        glUniform4fv(specularID, 1, combined_specular);
        glUniform1f(shineID, shininess);
    }
    // Now, we can draw two different ways - depending on whether or not the index buffer
    // is active. If indices is NULL, then draw triangles. Otherwise, draw elements
    if (!indices) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, total_vertices);
    }else {
        // 1) What kind of elements we're drawing
        // 2) The number of indices we're drawing (can be less than total_indices)
        // 3) The data type of the indices
        // 4) An offset within the buffer of where we should start drawing
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
        glDrawElements(GL_TRIANGLES, total_indices, GL_UNSIGNED_INT, NULL);
    }

    if (vertices) {
        glDisableVertexAttribArray(vPosition);    // Disable the variables
    }
    if (normals) {
        glDisableVertexAttribArray(vNormal);
    }
    if (vTexCoord != -1) {
        glDisableVertexAttribArray(vTexCoord);
        glDisable (GL_TEXTURE_2D);
    }
    if (vColor != -1) {
        glDisableVertexAttribArray(vColor);
    }
}

void Model::load_OBJ_file( const char* filename )
{
    WavefrontObjLoader loader( filename );
    if ( !loader.load( 4 ) ) {
        std::cout << "Failed to load OBJ file, exiting!" << std::endl;
        assert( !filename );
        exit( 1 );
    }
    assert( loader.num_vertices );
    set_geometry( loader.vertices, loader.num_vertices * 4 * sizeof( GLfloat ) );
    if ( loader.num_normals ) { set_normals( loader.normals, loader.num_normals * 3 * sizeof( GLfloat ) ); }
    if ( loader.num_uvs ) { set_texture_coordinates( loader.UVs, loader.num_uvs * 2 * sizeof( GLfloat ) ); }
    if ( loader.num_indices ) { set_index_buffer( loader.indices, loader.num_indices * sizeof( GLuint ) ); }
}
