#ifndef GRID_4T3D9HNV
#define GRID_4T3D9HNV

#include <GL/glew.h>

class Grid
{
    public:
        Grid();
        ~Grid();
        void draw();
        void setSubvision(int r);
    private:
        void destroy();
        GLuint vao;
        GLuint vbo;
        int r;
};

#endif /* end of include guard: GRID_4T3D9HNV */
