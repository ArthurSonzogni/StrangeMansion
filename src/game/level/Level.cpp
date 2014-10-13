#include "Level.hpp"
#include "LevelPart.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/Error.hpp"
#include "utils/glError.hpp"
#include "game/ShaderLib.hpp"

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
    
    //-----------------
    // load each levelPart
    //-----------------

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

    //----------------------------------
    // construct the portal destination
    //----------------------------------
    map<string,const PortalTransformed*> nameToPortal;
    for(auto& l : levelPart) 
    {
        for(auto& p : l->getPortals())
        {
            nameToPortal[p.from] = &p;
        }
    }
    for(auto& n : nameToPortal)
    {
        portalDestination[n.second] = nameToPortal[n.second->to];
    }
    for(auto& c : portalDestination)
    {
        cout << "---" << endl;
        cout << c.first->from
             << " -> "
             << c.second->to
             << endl;
        cout << c.first->to
             << " <- "
             << c.second->from
             << endl;
        cout << "---" << endl;
    }
}

Level::~Level() 
{
}
    
void Level::draw()
{
    //glEnable(GL_CULL_FACE);
    //for(auto l : levelPart)
    //{
        //l->draw();
    //}
    drawRecursivePortals(*levelPart[0], glm::mat4(1.0), glm::mat4(1.0), 1, 0);
}

void Level::draw(const glm::mat4& view, const glm::mat4& proj)
{
    //glEnable(GL_CULL_FACE);
    drawRecursivePortals(*levelPart[0], view, proj, 3, 0);
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

void Level::drawRecursivePortals(LevelPart& levelPart, glm::mat4 const &view, glm::mat4 const &projection, size_t maxRecursionLevel, size_t recursionLevel)
{
    for(auto& portal : levelPart.getPortals())
    {

        // check if orientation of the portal is okay
        glm::vec4 eyeToPortal = view * glm::vec4(portal.translation,1.0);
        glm::vec4 portalDirection = view*portal.view*glm::vec4(1.0,0.0,0.0,0.0);
        if (glm::dot(glm::vec3(eyeToPortal),glm::vec3(portalDirection)) > 0.001f)
            continue;
        

        PortalTransformed portalDest = *( portalDestination[&portal] );

        // _______________________________________
        //|                                       |
        //| draw the portal in the stencil buffer |
        //|_______________________________________|

        // shader use
        ShaderLib::geometry -> use();
        ShaderLib::geometry -> setUniform("view",view);
        ShaderLib::geometry -> setUniform("projection",projection);

        // color
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        // depth
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        // stencil
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, recursionLevel, 0xFF);
        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF);

        //// Draw portal into stencil buffer
        drawLevelBlockTransformed(portal.getLevelBlockTransformed());
        //portal.draw(viewMat, projMat);
        
        //ShaderLib::geometry -> setUniform("");
        //// Calculate view matrix as if the player was already teleported
        //glm::mat4 destView = viewMat * portal.modelMat()
            //* glm::rotate(glm::mat4(1.0f), M_PI, glm::vec3(0.0f, 1.0f, 0.0f) * portal.orientation())
            //* glm::inverse(portal.destination()->modelMat());
        glm::mat4 destView =
                view
            *   portal.view
            *   glm::rotate(glm::mat4(1.0f),float(M_PI),glm::vec3(0.0f,0.0f,1.0f))
            *   glm::inverse(portalDest.view)
            ;

        //// Base case, render inside of inner portal
        if (recursionLevel == maxRecursionLevel)
        {
            //continue;
            //// Enable color and depth drawing
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            
            //// Clear the depth buffer so we don't interfere with stuff
            //// outside of this inner portal
            glClear(GL_DEPTH_BUFFER_BIT);

            //// Enable the depth test
            //// So the stuff we render here is rendered correctly
            glEnable(GL_DEPTH_TEST);

            //// Enable stencil test
            //// So we can limit drawing inside of the inner portal
            glEnable(GL_STENCIL_TEST);

            //// Disable drawing into stencil buffer
            glStencilMask(0x00);

            //// Draw only where stencil value == recursionLevel + 1
            //// which is where we just drew the new portal
            glStencilFunc(GL_EQUAL, recursionLevel + 1, 0xFF);

            //// Draw scene objects with destView, limited to stencil buffer
            //// use an edited projection matrix to set the near plane to the portal plane
            //drawNonPortals(destView, portal.clippedProjMat(destView, projMat));
            ////drawNonPortals(destView, projMat);
            portalDest.levelPart.draw();
            
            //cout << (unsigned long)(portalDestination[&(portal)]) <<endl;
        }
        else
        {
            //// Recursion case
            //// Pass our new view matrix and the clipped projection matrix (see above)
            drawRecursivePortals(portalDest.levelPart,destView, projection, maxRecursionLevel, recursionLevel + 1);
            //drawRecursivePortals(portalDest.levelPart,destView, portalDest.clippedProjMat(destView,projection), maxRecursionLevel, recursionLevel + 1);
            //drawRecursivePortals(destView, portal.lippedProjMat(destView, projMat), maxRecursionLevel, recursionLevel + 1);
        }

        // shader use
        ShaderLib::geometry -> use();
        ShaderLib::geometry -> setUniform("view",view);
        ShaderLib::geometry -> setUniform("projection",projection);
        // color
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        // stencil
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glStencilFunc(GL_NOTEQUAL, recursionLevel + 1, 0xFF);
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

        //// Draw portal into stencil buffer
        drawLevelBlockTransformed(portal.getLevelBlockTransformed());
        //portal.draw(viewMat, projMat);
    }


    // _________________________
    //|                         |
    //| draw the portal's depth |
    //|_________________________|

    ShaderLib::geometry -> use();
    ShaderLib::geometry -> setUniform("view",view);
    ShaderLib::geometry -> setUniform("projection",projection);

    // color
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    // depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    // stencil
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    for (auto& portal : levelPart.getPortals())
    {
        // check if orientation of the portal is okay
        glm::vec4 eyeToPortal = view * glm::vec4(portal.translation,1.0);
        glm::vec4 portalDirection = view*portal.view*glm::vec4(1.0,0.0,0.0,0.0);
        if (glm::dot(glm::vec3(eyeToPortal),glm::vec3(portalDirection)) > 0.001f)
            continue;
        drawLevelBlockTransformed(portal.getLevelBlockTransformed());
    }

    // _________________
    //|                 |
    //| draw the level  |
    //|_________________|

    ShaderLib::geometry -> use();
    ShaderLib::geometry -> setUniform("view",view);
    ShaderLib::geometry -> setUniform("projection",projection);

    // color
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // depth
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    // stencil
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_EQUAL, recursionLevel, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    levelPart.draw();
}
