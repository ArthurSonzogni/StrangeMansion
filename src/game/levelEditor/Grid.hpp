#ifndef GRID_4T3D9HNV
#define GRID_4T3D9HNV

#include <GL/glew.h>

class ShaderProgram;

class Grid
{
    public:
        Grid();
        ~Grid();
        void draw();
        void setSubvision(int r);
        ShaderProgram& getShader();
    private:
        void destroy();
        GLuint vao;
        GLuint vbo;
        ShaderProgram& shader;
        int r;
};

#endif /* end of include guard: GRID_4T3D9HNV */
