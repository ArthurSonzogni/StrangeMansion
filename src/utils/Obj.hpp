#ifndef OBJ_YFJJB6N0
#define OBJ_YFJJB6N0

#include <GL/glew.h>
#include "utils/glError.hpp"

class Obj
{
    public:
        Obj(const char * filename);
        ~Obj();
        GLuint getVBO() const;
        unsigned int getNbFace() const;
    private:
        GLuint vbo;
        unsigned int faceNb;
};

#endif /* end of include guard: OBJ_YFJJB6N0 */
