#include "Level.hpp"
#include "LevelPart.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/Error.hpp"
#include "utils/glError.hpp"

using namespace std;

Level::Level(const string& _folder):
    folder(_folder)
{
    cout<<"[Loading] "<<folder<<endl;
    ifstream file;
    file.open(folder+"/index.map");
    if (not file.is_open())
    {
        cerr<<"[Error] the file "+folder+"/index.map does'nt exist"<<endl;
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

        if (firstWord == "LevelPart")
        {
            string filename;
            sline >> filename;
            levelPart.push_back(new LevelPart(folder+"/"+filename));
        }
        else 
        {
            cerr<<"Error when reading line "<<lineNb<<" of the file "<<folder+"/index.map"<<endl;
            cerr<<"->"<<line;
        }
    }

    file.close();
}

Level::~Level() 
{
}
    
void Level::draw()
{
    //glEnable(GL_CULL_FACE);
    for(auto l : levelPart)
    {
        l->draw();
    }
}

void Level::addLevelPart(const std::string& name)
{
    throw Error(__LINE__,__FILE__,"Not implemented");
}
void Level::removeLevelPart(const std::string& name)
{
    throw Error(__LINE__,__FILE__,"Not implemented");
}
void Level::selectCurrentLevelPart(int n)
{
    throw Error(__LINE__,__FILE__,"Not implemented");
}
void Level::drawBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz)
{
    levelPart[0]->drawBlockGhost(name,x,y,z,rx,ry,rz);
}
void Level::addBlock(const std::string& name, float x, float y, float z, float rx, float ry, float rz)
{
    levelPart[0]->addBlock(name,x,y,z,rx,ry,rz);
}

void Level::save()
{
    levelPart[0]->save();
}

bool Level::testBlock(const glm::vec3 p0,const glm::vec3 p1)
{
    return levelPart[0]->testBlock(p0,p1);
}

void Level::drawRecursivePortals(glm::mat4 const &viewMat, glm::mat4 const &projMat, size_t maxRecursionLevel, size_t recursionLevel)
{

}
