#ifndef GLCONTAINER_V7NW4TJN
#define GLCONTAINER_V7NW4TJN

#include "utils/glm.hpp"
#include <GL/glew.h>

namespace GLContainer
{
    struct Vertice
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
        Vertice(glm::vec3 p, glm::vec3 n, glm::vec2 t):
            position(p),normal(n),texCoord(t){}
        Vertice(){}
    };

    struct Element
    {
        GLuint v1,v2,v3;
        Element(GLuint a, GLuint b, GLuint c):v1(a),v2(b),v3(c){}
    };
}

#endif /* end of include guard: GLCONTAINER_V7NW4TJN */
