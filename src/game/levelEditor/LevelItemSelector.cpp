#include "LevelEditor.hpp"
#include "LevelItemSelector.hpp"
#include "graphic/GLContainer.hpp"
#include "graphic/Shader.hpp"

using namespace std;

LevelItemSelector::LevelItemSelector(LevelEditor& _levelEditor):
    levelEditor(_levelEditor),
    shader(ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ))
{
}

void LevelItemSelector::draw()
{

}


void LevelItemSelector::build()
{
    vector<GLContainer::Vertice> vertice; 

    // vbo
    glGenBuffers( 1, &vbo );
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLContainer::Vertice)*vertice.size(), &vertice[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError(__FILE__,__LINE__);

    // vao
    glGenVertexArrays( 1, &vao);
    glBindVertexArray(vao);

        // vbo
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        shader.setAttribute("position", 3, GL_FALSE, 8, 0);
        shader.setAttribute("normal"  , 3, GL_FALSE, 8, 3);
        shader.setAttribute("texCoord", 2, GL_FALSE, 8, 6);

    glBindVertexArray(0);
    glCheckError(__FILE__,__LINE__);
}
