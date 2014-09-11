#include "Grid.hpp"
#include "utils/glError.hpp"
#include "game/ShaderLib.hpp"
#include <vector>

using namespace std;

Grid::Grid()
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
    ShaderLib::geometryColor -> use();

    glBindVertexArray(vao);
     
	glDrawArrays(
		GL_LINES,      // mode
        0,                 // first
		(2*r+1)*2*2           // count
	);

    glBindVertexArray(0);
    ShaderLib::geometryColor -> unuse();
}

void Grid::setSubvision(int _r)
{
    destroy();
    r=_r;

    vector<GLfloat> vertices;
    for(int x = -r; x<=r; ++x)
    {
        vertices.push_back(x-0.5);
        vertices.push_back(-r-0.5);
        vertices.push_back(-0.5);
        vertices.push_back(x-0.5);
        vertices.push_back(r-0.5);
        vertices.push_back(-0.5);
    }
    for(int y = -r; y<=r; ++y)
    {
        vertices.push_back(-r-0.5);
        vertices.push_back(y-0.5);
        vertices.push_back(-0.5);
        vertices.push_back(r-0.5);
        vertices.push_back(y-0.5);
        vertices.push_back(-0.5);
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
    ShaderLib::geometryColor -> setAttribute("position", 3, GL_FALSE, 3, 0);
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
