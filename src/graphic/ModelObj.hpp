#ifndef MODELOBJ_IZ6KZI5P
#define MODELOBJ_IZ6KZI5P

#include <GL/glew.h>
#include "utils/Texture.hpp"

class ShaderProgram;

class ModelObj
{
    public:
        ModelObj(const char * filename, ShaderProgram& shader);
        ModelObj(const char * filename);
        ~ModelObj();
        void draw();
        ShaderProgram& getShader();
    private:
        bool hasTexture;
        GLuint vbo;
        GLuint vao;
        int faceNb;
        ShaderProgram& shader;
        Texture& texture;
};

#endif /* end of include guard: MODELOBJ_IZ6KZI5P */
