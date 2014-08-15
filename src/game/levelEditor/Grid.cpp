#include "Grid.hpp"
#include "utils/glError.hpp"
#include "graphic/Shader.hpp"
#include <vector>

using namespace std;

Grid::Grid():
    shader(ShaderProgram::loadFromFile(
        "shader/geometryPassWhite.vert",
        "shader/geometryPassWhite.frag"
    ))
{
    vbo=0;
    vao=0;
    r=5;
    setSubvision(r);
}

Grid::~Grid()
{
    destroy();
}

void Grid::draw()
{
    shader.use();

    glBindVertexArray(vao);
     
	glDrawArrays(
		GL_LINES,      // mode
        0,                 // first
		(2*r+1)*2*2           // count
	);

    glBindVertexArray(0);
    shader.unuse();
}

void Grid::setSubvision(int _r)
{
    destroy();
    r=_r;

    vector<GLfloat> vertices;
    for(int x = -r; x<=r; ++x)
    {
        vertices.push_back(x);
        vertices.push_back(-r);
        vertices.push_back(0.0);
        vertices.push_back(x);
        vertices.push_back(r);
        vertices.push_back(0.0);
    }
    for(int y = -r; y<=r; ++y)
    {
        vertices.push_back(-r);
        vertices.push_back(y);
        vertices.push_back(0.0);
        vertices.push_back(r);
        vertices.push_back(y);
        vertices.push_back(0.0);
    }

    // vbo allocation
    glGenBuffers( 1, &vbo );

    // vbo filling
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError(__FILE__,__LINE__);

    // vao allocation
    glGenVertexArrays(1,&vao);
    glCheckError(__FILE__,__LINE__);

    // vao filling
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    shader.setAttribute("position", 3, GL_FALSE, 3, 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glCheckError(__FILE__,__LINE__);

}

void Grid::destroy()
{
    if (vbo)
    {
        glDeleteBuffers(1,&vbo);
        vbo=0;
    }
    if(vao)
    {
        glDeleteVertexArrays(1,&vao);
        vao=0;
    }
}

ShaderProgram& Grid::getShader()
{
    return shader;
}
