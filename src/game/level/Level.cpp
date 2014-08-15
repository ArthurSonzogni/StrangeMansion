#include "Level.hpp"
#include "LevelPart.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
    for(auto l : levelPart)
    {
        l->draw();
    }
}
