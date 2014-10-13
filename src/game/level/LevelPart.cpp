#include "LevelPart.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/glError.hpp"
#include "game/ShaderLib.hpp"
#include "utils/Error.hpp"
#include <algorithm>
#include "math/collision.hpp"
#include <iostream>

using namespace std;


bool LevelBlockTransformed::sortByTexture(const LevelBlockTransformed* a, const LevelBlockTransformed* b)
{
    return &(a->block.getTexture()) < &(b->block.getTexture());
}

LevelPart::LevelPart(const string& _filename):
    vbo(0),
    vao(0),
    filename(_filename)
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
        else if (firstWord == "portal")
        {
            string portalName;
            float x,y,z,rx,ry,rz;
            string from,to;
            sline >> portalName >> from >> to >> x >> y >> z >> rx >> ry >> rz;
            addPortal(portalName,from,to,x,y,z,rx,ry,rz);
        }
        else
        {
            stringstream err;
            err<<"Can't parse:\""<<line<<"\" file:"<<filename<<" line:"<<lineNb;
            throw Error(__LINE__,__FILE__,err.str());
        }
    }
    
    // constr
    file.close();

    build();
}

glm::mat4 getTransformation(float x, float y, float z, float rx, float ry, float rz)
{
    glm::mat4 transformation(1.0);
    transformation = glm::translate(transformation,{x,y,z});
    transformation = glm::rotate(transformation,float(rz*M_PI),{0.f,0.f,1.f});
    transformation = glm::rotate(transformation,float(ry*M_PI),{0.f,1.f,0.f});
    transformation = glm::rotate(transformation,float(rx*M_PI),{1.f,0.f,0.f});
    return transformation;
}

glm::mat4 getTransformation(const glm::vec3 translation, const glm::vec3 rotation)
{
    glm::mat4 transformation(1.0);
    transformation = glm::translate(transformation,translation);
    transformation = glm::rotate(transformation,float(rotation.z*M_PI),{0.f,0.f,1.f});
    transformation = glm::rotate(transformation,float(rotation.y*M_PI),{0.f,1.f,0.f});
    transformation = glm::rotate(transformation,float(rotation.x*M_PI),{1.f,0.f,0.f});
    return transformation;
}

void LevelPart::build()
{
    glCheckError(__FILE__,__LINE__);

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
        const vector<GLContainer::Vertice>& blockVertices = it->block.getVertices();
        index+=blockVertices.size();
        texture.back().howMany+=blockVertices.size();
        for(auto& v : blockVertices)
        {
            GLContainer::Vertice vTransformed;

            glm::mat4 transformation = getTransformation(it->translation,it->rotation);
            glm::vec4 newPosition = (transformation) * glm::vec4(v.position,1.0);
            glm::vec4 newNormal   = (transformation) * glm::vec4(v.normal  ,0.0);
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
        ShaderLib::geometry -> setAttribute("position", 3, GL_FALSE, 8, 0);
        ShaderLib::geometry -> setAttribute("normal"  , 3, GL_FALSE, 8, 3);
        ShaderLib::geometry -> setAttribute("texCoord", 2, GL_FALSE, 8, 6);

    glBindVertexArray(0);
    glCheckError(__FILE__,__LINE__);

}

void LevelPart::draw()
{
    //ShaderLib::geometry.setUniform("texture0",0);

    ShaderLib::geometry -> use();
    glBindVertexArray(vao);
    for(auto& t : texture)
    {
        t.texture.bind(GL_TEXTURE0);
        glDrawArrays(
            GL_TRIANGLES,      // mode
            t.index,           // first
            t.howMany          // count
        );
    }
    glBindVertexArray(0);
    ShaderLib::geometry -> unuse();
}

void LevelPart::addBlockInternal(const string& blockName, float x, float y, float z, float rx, float ry, float rz)
{
    levelBlockTransformed.push_back({
        LevelBlock::loadFromName(blockName),
        {x,y,z},
        {rx,ry,rz}
    });
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

void LevelPart::save()
{
    cout << "save Levelpart to " << filename << endl;

    ofstream myfile;
    myfile.open(filename);
    for(auto& it : levelBlockTransformed)
    {
        const glm::vec3& t = it.translation;
        const glm::vec3& r = it.rotation;
        myfile
            << "block " << (it.block.getName())
            << " " << t.x << " " << t.y << " " << t.z
            << " " << r.x << " " << r.y << " " << r.z
            << endl;
    }
    myfile.close();
}

bool LevelPart::testBlock(const glm::vec3& p0,const glm::vec3& p1)
{
    unsigned int n = vertice.size();
    for(unsigned int i = 0 ; i < n ; )
    {
        glm::vec3 t1 = vertice[i++].position;
        glm::vec3 t2 = vertice[i++].position;
        glm::vec3 t3 = vertice[i++].position;
        if ( rayTriangleCollision(p0,p1,t1,t2,t3))
            return true;
    }
    return false;
}

void LevelPart::addPortal(string portalName,string from, string to,float x,float y,float z,float rx,float ry,float rz)
{
    const glm::mat4 viewMat = getTransformation(x,y,z,rx,ry,rz);

    portalTransformed.push_back({
        Portal::loadFromName(portalName),
        from,
        to,
        {x,y,z},
        {rx,ry,rz},
        viewMat,
        *this
    });
}

const std::vector<PortalTransformed>& LevelPart::getPortals() const
{
    return portalTransformed;
}

void LevelPart::drawBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz)
{
    drawLevelBlockTransformed({
        LevelBlock::loadFromName(name),
        {x,y,z},
        {rx,ry,rz}
    });
}

////////////////////

void drawLevelBlockTransformed(const LevelBlockTransformed& l)
{
    GLuint vaoGhost,vboGhost;

    // create the verticeGhost array
    vector<GLContainer::Vertice> verticeGhost;
    for(auto& v : l.block.getVertices())
    {
        GLContainer::Vertice vTransformed;

        glm::mat4 transformation = getTransformation(l.translation,l.rotation);

        glm::vec4 newPosition = (transformation) * glm::vec4(v.position,1.0);
        glm::vec4 newNormal   = (transformation) * glm::vec4(v.normal  ,0.0);
        verticeGhost.push_back( GLContainer::Vertice( glm::vec3(newPosition), glm::vec3(newNormal), v.texCoord )); 
    }

    //-----------------
    // VBO/VAO creation
    //-----------------

    // vboGhost
    glGenBuffers( 1, &vboGhost );
    glBindBuffer(GL_ARRAY_BUFFER, vboGhost);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLContainer::Vertice)*verticeGhost.size(), &verticeGhost[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError(__FILE__,__LINE__);

    // vaoGhost
    glGenVertexArrays( 1, &vaoGhost);
    glBindVertexArray(vaoGhost);

        // vboGhost
        glBindBuffer(GL_ARRAY_BUFFER,vboGhost);
        ShaderLib::geometry -> setAttribute("position", 3, GL_FALSE, 8, 0);
        ShaderLib::geometry -> setAttribute("normal"  , 3, GL_FALSE, 8, 3);
        ShaderLib::geometry -> setAttribute("texCoord", 2, GL_FALSE, 8, 6);

    glBindVertexArray(0);
    glCheckError(__FILE__,__LINE__);

    //-----------------
    // Drawing
    //-----------------
    
    ShaderLib::geometry -> use();
    glBindVertexArray(vaoGhost);
    l.block.getTexture().bind(GL_TEXTURE0);
    glDrawArrays(
        GL_TRIANGLES,      // mode
        0,                 // first
        verticeGhost.size()     // count
    );
    glBindVertexArray(0);
    ShaderLib::geometry -> unuse();

    //-----------------
    // VBO/VAO destruction
    //-----------------
    glDeleteVertexArrays(1,&vaoGhost);
    glDeleteBuffers(1,&vboGhost);
}

glm::mat4 PortalTransformed::clippedProjMat(glm::mat4 const &viewMat, glm::mat4 const &projMat) const
{
    float dist = glm::length(translation);
    //glm::vec4 clipPlane(glm::vec3(0.0f, 0.0f, -1.0f), dist);
    glm::vec4 clipPlane( glm::vec3(0.0f, 0.0f, -1.0f), dist);
    clipPlane = -glm::inverse(glm::transpose(viewMat)) * clipPlane;

    //if (clipPlane.w > 0.0f)
        return projMat;

    glm::vec4 q = glm::inverse(projMat) * glm::vec4(
        glm::sign(clipPlane.x),
        glm::sign(clipPlane.y),
        1.0f,
        1.0f
    );

    glm::vec4 c = clipPlane * (2.0f / (glm::dot(clipPlane, q)));

    glm::mat4 newProj = projMat;
    // third row = clip plane - fourth row
    newProj = glm::row(newProj, 2, c - glm::row(newProj, 3));

    return newProj;
} 
