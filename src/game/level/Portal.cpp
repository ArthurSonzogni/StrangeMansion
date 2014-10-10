#include "Portal.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "utils/file.hpp"
#include <iostream>
#include "utils/Error.hpp"


using namespace std;


bool Portal::namesLoaded = false;
map<string,Portal*> portalByNames;
void Portal::loadAllNames()
{
    string folder("./portal/");
    vector<string> v = directoryListFiles(folder);
    for(auto& vv : v)
    {
        cout<<vv<<endl;
        Portal& portal(Portal::loadFromFile(folder+vv));
        portalByNames[portal.getName()] = &portal;
    }
}

map<string,Portal*> loadedPortal;
Portal& Portal::loadFromFile(std::string filename)
{
    auto it = loadedPortal.find(filename);
    if (it!=loadedPortal.end())
        return *(it->second);

    Portal* portal = new Portal(filename);
    loadedPortal[filename] = portal;
    return *portal;
}

Portal::Portal(std::string filename)
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

    transformation = glm::mat4(1.0);

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
            sline >> blockName;
            geometry = &LevelBlock::loadFromName(blockName);
        }
        else if (firstWord == "name")
        {
            sline >> name; 
        }
        else if (firstWord == "translation")
        {
            float x,y,z;
            sline >> x >> y >> z;
            transformation = glm::translate(transformation,{x,y,z});
        }
        else if (firstWord == "rotation")
        {
            float rx,ry,rz;
            sline >> rx >> ry >> rz;
            transformation = glm::rotate(transformation,float(rz*M_PI),{0.f,0.f,1.f});
            transformation = glm::rotate(transformation,float(ry*M_PI),{0.f,1.f,0.f});
            transformation = glm::rotate(transformation,float(rx*M_PI),{1.f,0.f,0.f});
        }
    }
}

Portal& Portal::loadFromName(std::string name)
{
    if (!namesLoaded)
    {
        loadAllNames();
        namesLoaded = true;
    }
    auto it = portalByNames.find(name);
    if (it != portalByNames.end())
    {
        return *(it->second);
    }
    else
    {
        throw Error(__LINE__,__FILE__,"The portal "+name+" doesn't exist");
    }
}


// getter
const std::string& Portal::getName() const
{
    return name;
}
const LevelBlock& Portal::getGeometry() const
{
    return *geometry;
}
const glm::mat4& Portal::getTransformation() const
{
    return transformation;
}

glm::mat4 const Portal::clippedProjMat(glm::mat4 const &viewMat, glm::mat4 const &projMat) const
{
    float dist = glm::length(d_position);
    glm::vec4 clipPlane(d_orientation * glm::vec3(0.0f, 0.0f, -1.0f), dist);
    clipPlane = glm::inverse(glm::transpose(viewMat)) * clipPlane;

    if (clipPlane.w > 0.0f)
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
