#include "Level.hpp"
#include "LevelPart.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/Error.hpp"

using namespace std;

Level::Level(const string& folder)
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
void Level::addBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz)
{
    levelPart[0]->addBlockGhost(name,x,y,z,rx,ry,rz);
}
void Level::addBlock(const std::string& name, float x, float y, float z, float rx, float ry, float rz)
{
    levelPart[0]->addBlock(name,x,y,z,rx,ry,rz);
}

