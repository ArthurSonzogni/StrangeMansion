#include "ModelObj.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <clocale>
#include <cstdio>
#include <string>
#include "Shader.hpp"
#include "utils/glError.hpp"

using namespace std;


ModelObj::ModelObj(const char * filename):
    ModelObj(filename,ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ))
{
    hasTexture = true;
}

ModelObj::ModelObj(const char * filename, ShaderProgram& _shader):
    obj(filename),
    hasTexture(false),
    shader(_shader),
    texture(Texture::loadFromFile(
        "texture/texture.png"
    ))
{
    // vao allocation
    glGenVertexArrays(1,&vao);

    glCheckError(__FILE__,__LINE__);

    // vao filling
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,obj.getVBO());
    shader.setAttribute("position", 3, GL_FALSE, 8, 0);
    shader.setAttribute("normal"  , 3, GL_FALSE, 8, 3);
    shader.setAttribute("texCoord"   , 2, GL_FALSE, 8, 6);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glCheckError(__FILE__,__LINE__);
}

ModelObj::~ModelObj()
{
    glDeleteVertexArrays(1,&vao);
}


void ModelObj::draw()
{
    shader.use();

    if (hasTexture)
    {
        texture.bind(GL_TEXTURE0);
        shader.setUniform("texture0",0);
    }

    glBindVertexArray(vao);

     
	glDrawArrays(
		GL_TRIANGLES,      // mode
        0,                 // first
		obj.getNbFace()*3           // count
	);

    glBindVertexArray(0);
    shader.unuse();
}

ShaderProgram& ModelObj::getShader()
{
    return shader;
}
