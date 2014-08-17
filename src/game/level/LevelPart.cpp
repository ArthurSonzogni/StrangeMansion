#include "LevelPart.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/glError.hpp"
#include "graphic/Shader.hpp"
#include "utils/Error.hpp"
#include <algorithm>

using namespace std;

#define M_PI 3.14159265358979323846f

bool LevelBlockTransformed::sortByTexture(const LevelBlockTransformed* a, const LevelBlockTransformed* b)
{
    return &(a->block.getTexture()) < &(b->block.getTexture());
}

LevelPart::LevelPart(const string& filename):
    shader(ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    )),
    vbo(0),
    vao(0)
{
    ifstream file;
    file.open(filename);
    if (not file.is_open())
    {
        cerr<<"[Error] the file "<<filename<<" doesn't exist"<<endl;
        exit(EXIT_FAILURE);
    }


	//string line;
    int lineNb = 0;
    string line;
	while(getline(file,line))
    {
        lineNb++;

        stringstream sline;
        string firstWord;

        sline << line;
        sline >> firstWord;

        if (firstWord == "block")
        {
            string blockName;
            float x,y,z,rx,ry,rz;
            sline >> blockName >> x >> y >> z >> rx >> ry >> rz;
            addBlockInternal(blockName,x,y,z,rx,ry,rz);
        }
        else
        {
            stringstream err;
            string error;
            err<<"Can't parse:\""<<line<<"\" file:"<<filename<<" line:"<<line;
            err>>error;
            throw Error(__LINE__,__FILE__,error);
        }
    }
    
    // constr
    file.close();

    build();
}

void LevelPart::build()
{
    deleteBuffer();

    // sort levelBlock by texture
    vector<LevelBlockTransformed*> sortedLevelBlockTransformed;
    for(auto& it : levelBlockTransformed)
    {
        sortedLevelBlockTransformed.push_back(&it);
    }
    std::sort(
        sortedLevelBlockTransformed.begin(),
        sortedLevelBlockTransformed.end(),
        LevelBlockTransformed::sortByTexture
    );

    // build the vbo vector and texture vector
    texture.clear();
    texture.push_back(TextureContainer( Texture::loadFromFile("texture/texture.png"),0,0));
    vertice.clear();
    int index = 0;
    for(auto it : sortedLevelBlockTransformed)
    {
        if ( &(texture.back().texture) != &(it->block.getTexture()))
        {
            texture.push_back(TextureContainer(it->block.getTexture(),index,0));
        }
        const vector<GLContainer::Vertice>& blockVertices =it->block.getVertices();
        index+=blockVertices.size();
        texture.back().howMany+=blockVertices.size();
        for(auto& v : blockVertices)
        {
            GLContainer::Vertice vTransformed;
            glm::vec4 newPosition = (it->transformation) * glm::vec4(v.position,1.0);
            glm::vec4 newNormal   = (it->transformation) * glm::vec4(v.normal  ,0.0);
            vertice.push_back( GLContainer::Vertice( glm::vec3(newPosition), glm::vec3(newNormal), v.texCoord )); 
        }
    }

    verticeNb = vertice.size();

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

void LevelPart::draw()
{
    Texture& texture(Texture::loadFromFile(
        "texture/texture.png"
    ));
    texture.bind(GL_TEXTURE0);
    shader.setUniform("texture0",0);

    shader.use();
    glBindVertexArray(vao);
        glDrawArrays(
            GL_TRIANGLES,      // mode
            0,                 // first
            verticeNb          // count
        );
    glBindVertexArray(0);
    shader.unuse();
}

void LevelPart::addBlockInternal(const string& blockName, float x, float y, float z, float rx, float ry, float rz)
{
    glm::mat4 transformation(1.0);
    transformation = glm::translate(transformation,{x,y,z});
    transformation = glm::rotate(transformation,rz*M_PI,{0.f,0.f,1.f});
    transformation = glm::rotate(transformation,ry*M_PI,{0.f,1.f,0.f});
    transformation = glm::rotate(transformation,rx*M_PI,{1.f,0.f,0.f});
    levelBlockTransformed.push_back({
        LevelBlock::loadFromName(blockName),
        transformation
    });
}

void LevelPart::addBlockGhost(const string& blockName, float x, float y, float z, float rx, float ry, float rz)
{
    if (levelBlockTransformed.size() == 0) return;
    levelBlockTransformed.pop_back();
    addBlockInternal(blockName,x,y,z,rx,ry,rz);
    build();
}

void LevelPart::addBlock(const string& blockName, float x, float y, float z, float rx, float ry, float rz)
{
    addBlockInternal(blockName,x,y,z,rx,ry,rz);
    build();
}

LevelPart::~LevelPart()
{
    deleteBuffer();
}

void LevelPart::deleteBuffer()
{
    if (vbo)
    {
        glDeleteBuffers(1,&vbo);
        vbo = 0;
    }
    if (vao)
    {
        glDeleteVertexArrays(1,&vao);
        vao = 0;
    }
}
