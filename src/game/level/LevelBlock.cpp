#include "LevelBlock.hpp"
#include "utils/Error.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "utils/file.hpp"

using namespace std;

bool LevelBlock::namesLoaded;

LevelBlock::LevelBlock(string filename)
{
    ifstream file;
    file.open(filename);
    if (not file.is_open())
    {
        throw Error(__LINE__,__FILE__,"The file "+filename+" doesn't exist");
    }

	//string line;
    int lineNb = 0;
    string line;
    vector<GLContainer::Vertice> tempVertices;
    vector<GLContainer::Element> tempElements;

	while(getline(file,line))
    {
        lineNb++;

        stringstream sline;
        string firstWord;

        sline << line;
        sline >> firstWord;

        if (firstWord == "name")
        {
            sline >> name;
        }
        else if (firstWord == "texture")
        {
            string textureFilename;
            sline >> textureFilename;
            texture = &Texture::loadFromFile(textureFilename);
        }
        else if (firstWord == "vertice")
        {
            float px,py,pz,nx,ny,nz,tx,ty;
            sline >> px >> py >> pz >> nx >> ny >> nz >> tx >> ty;
            tempVertices.push_back(GLContainer::Vertice(
                glm::vec3(px,py,pz),
                glm::vec3(nx,ny,nz),
                glm::vec2(tx,ty)
            ));
        }
        else if (firstWord == "face")
        {
            GLuint v1, v2, v3;
            sline >> v1 >> v2 >> v3;
            tempElements.push_back(GLContainer::Element(v1,v2,v3));
        }
        else 
        {
            throw Error(__LINE__,__FILE__,"can't parse "+filename+" line : "+line+" "+line);
        }
    }

    // construct the final vertice array
    GLuint n = tempVertices.size();
    for(auto& it : tempElements)
    {
        // check element index;
        if( it.v1 >= n &&
            it.v2 >= n &&
            it.v3 >= n     )
        {
            throw Error(__LINE__,__FILE__,"face index invalid in file"+filename);
        }
        // pushing
        vertices.push_back(tempVertices[it.v1]);
        vertices.push_back(tempVertices[it.v2]);
        vertices.push_back(tempVertices[it.v3]);
    }
}

const vector<GLContainer::Vertice>&  LevelBlock::getVertices() const
{
   return vertices;
}
const std::string& LevelBlock::getName() const
{
    return name;
}
Texture&  LevelBlock::getTexture() const
{
    return *texture;
}
    
map<string,LevelBlock*> loadedBlock;
LevelBlock& LevelBlock::loadFromFile(std::string filename)
{
    auto it = loadedBlock.find(filename);
    if (it!=loadedBlock.end())
        return *(it->second);
    
    LevelBlock* block = new LevelBlock(filename);
    loadedBlock[filename] = block;
    return *block;
}

map<string,LevelBlock*> levelBlockByNames;
LevelBlock& LevelBlock::loadFromName(std::string name)
{
    if (!namesLoaded)
    {
        loadAllNames();
        namesLoaded = true;
    }
    auto it = levelBlockByNames.find(name);
    if (it != levelBlockByNames.end())
    {
        return *(it->second);
    }
    else
    {
        throw Error(__LINE__,__FILE__,"The block "+name+" doesn't exist");
    }
}

void LevelBlock::loadAllNames()
{
    string folder("./block/");
    vector<string> v = directoryListFiles(folder);
    for(auto& vv : v)
    {
        cout<<vv<<endl;
        LevelBlock& levelBlock(LevelBlock::loadFromFile(folder+vv));
        levelBlockByNames[levelBlock.getName()] = &levelBlock;
    }
}
