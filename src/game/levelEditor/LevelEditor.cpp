#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LevelEditor.hpp"
#include <iostream>
#include "game/ShaderLib.hpp"
#include "utils/glError.hpp"
#include "utils/Texture.hpp"
#include "system/Input.hpp"
#include <cmath>

using namespace std;



LevelEditor::LevelEditor():
    Application(),
    obj("obj/Charmander.obj"),
    screenObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/compose.vert",
        "shader/compose.frag"
    )),
    framebuffer(getWidth(),getHeight(),3),
    level("map/2"),
    drawingMode(false)
{
    glCheckError(__FILE__,__LINE__);
    assignKeysMapping();
    projection = glm::perspective(70.0f, getWindowRatio(), 0.1f, 100.f);
    view  = glm::lookAt(
        glm::vec3(0.0,6.0,6.0),
        glm::vec3(0.0,0.0,3.0),
        glm::vec3(0.0,0.0,1.0)
    );
    currentRotation = 0;
    currentBlock="sol";
}

void LevelEditor::step()
{
    // escape
    if (Input::isKeyPressed(keys[KEY_ESCAPE]))
        exit();
    // fps
    static float deltaMean = getFrameDeltaTime();
    static int n = 0;
    deltaMean = 0.9*deltaMean + 0.1*getFrameDeltaTime();
    if (n++%100==0)
        cout << "fps=" << (int)(1/deltaMean) << endl;

    if (windowDimensionChange())
    {
        framebuffer.recreate(getWidth(),getHeight());
        projection = glm::perspective(70.0f, getWindowRatio(), 0.1f, 100.f);
        cout<<getWidth()<<" "<<getHeight()<<endl;
    }

    Input::update(getWindow());
    cameraEvent();

    drawingMode = Input::isKeyHold(GLFW_KEY_SPACE);

    float mouseX = 2.0*Input::mouseX()/getWidth() -1.0;
    float mouseY = -2.0*Input::mouseY()/getHeight() +1.0;

    // mouse put element
    {
        glm::vec3 v = mouseIntersectCurrentPlan({mouseX,mouseY});
        float x = v.x;
        float y = v.y;
        float z = 0;
        if (Input::isKeyHold(GLFW_KEY_T))
        {
            float precision = 4;
            x = round(x*precision)/precision;
            y = round(y*precision)/precision;
        }
        else
        {
            x = round(x);
            y = round(y);
        }
        if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
            level.addBlock(currentBlock,x,y,z,0.0,0.0,currentRotation*0.5);
        else
            ghostBlock = {true,currentBlock,(float)x,(float)y,(float)z,0.0,0.0,currentRotation*0.5f};
    }

    // mouse translation
    {
        float limit = 0.9;
        float power = 1.0 * getFrameDeltaTime() / (1.0 - limit);


        glm::mat4 invView = glm::inverse(view);
        glm::vec4 viewRay = invView * glm::vec4(0.0,0.0,1.0,0.0);
        glm::vec4 rightRay = invView* glm::vec4(1.0,0.0,0.0,0.0);

        // projection
        viewRay.z = 0;
        viewRay = power * glm::normalize(viewRay);

        // projection
        rightRay.z = 0;
        rightRay = power * glm::normalize(rightRay);

        if (mouseX<-limit) view = glm::translate(view,-(mouseX-limit)*glm::vec3(rightRay));
        if (mouseX>+limit) view = glm::translate(view,-(mouseX+limit)*glm::vec3(rightRay));
        if (mouseY<-limit) view = glm::translate(view,+(mouseY-limit)*glm::vec3(viewRay));
        if (mouseY>+limit) view = glm::translate(view,+(mouseY+limit)*glm::vec3(viewRay));
    }

    // saving
    {
        if (Input::isKeyPressed(keys[KEY_SAVE]))
        {
            level.save();
        }
    }

    if (Input::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        currentRotation = (currentRotation + 1) % 4;

    if (Input::isKeyPressed(GLFW_KEY_TAB))
    {
        static int i = 0;
        switch(i++%4)
        {
            case 0 :  currentBlock = "sol"; break;
            case 1 :  currentBlock = "corner"; break;
            case 2 :  currentBlock = "wall"; break;
            case 3 :  currentBlock = "roof"; break;
            default: break;
        }
    }

    // triangle collision
    {
        //glm::mat4 inverse = glm::inverse(projection*view);
        //glm::vec4 m1 = inverse*glm::vec4(mouseX,mouseY,+0.0,1.0);
        //glm::vec4 m2 = inverse*glm::vec4(mouseX,mouseY,+1.0,1.0);

        //m1 = m1/m1.w;
        //m2 = m2/m2.w;

        //glm::vec3 M1 = glm::vec3(m1);
        //glm::vec3 M2 = glm::vec3(m2);

        //cout << level.testBlock(M1,M2) << endl;
    }


}

void LevelEditor::draw()
{
    //return;
    //=================================
    //return;
    framebuffer.bindToWrite();
    //return;

    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);


    static float t=0;
    t+=getFrameDeltaTime();
    ShaderLib::geometry -> use();
    ShaderLib::geometry -> setUniform("projection", projection);
    ShaderLib::geometry -> setUniform("view", view);
    ShaderLib::geometry -> setUniform("model", glm::mat4(1.0));
    level.draw(view,projection);

    // block ghost drawing
    if (ghostBlock.enabled)
    {
        const GhostBlock& g = ghostBlock;// alias
        level.drawBlockGhost(g.blockName,g.x,g.y,g.z,g.rx,g.ry,g.rz);
    }


    // grid
    //glDisable(GL_DEPTH_TEST);
    ShaderLib::geometryColor -> use();
    ShaderLib::geometryColor -> setUniform("projection", projection);
    ShaderLib::geometryColor -> setUniform("view", view);
    ShaderLib::geometryColor -> setUniform("model", glm::mat4(1.0));
    grid.draw();

    //return;
    if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
        static int ii=0;
        ii++;
        grid.setSubvision(ii);
    }

    //=================================
    if (drawingMode) {
        framebuffer.drawToScreen();
    } else {
        screenObj.getShader().use();
        framebuffer.bindToRead();
        glDisable(GL_DEPTH_TEST);
        screenObj.getShader().setUniform("positionMap",0);
        screenObj.getShader().setUniform("colorMap",1);
        screenObj.getShader().setUniform("normalMap",2);
        screenObj.getShader().setUniform("lightDir",glm::normalize(glm::vec3(view*glm::vec4(2.0,0.0,-1.0,0.0))));


        //glDepthMask(GL_FALSE);
        //glDisable(GL_DEPTH_TEST);
        //glEnable(GL_BLEND);
        //glBlendEquation(GL_FUNC_ADD);
        //glBlendFunc(GL_ONE, GL_ONE);
        
        screenObj.draw();
    }

}

void LevelEditor::loop()
{
    step();
    draw();
}

void LevelEditor::cameraEvent()
{
    float tDelta = 4.0 * getFrameDeltaTime();
    float rDelta = 2.0 * getFrameDeltaTime();

    // translation
    if (Input::isKeyHold(keys[KEY_CAMERA_LEFT]))
        view = glm::translate(glm::mat4(1.0),glm::vec3(+tDelta,0.0,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_RIGHT]))
        view = glm::translate(glm::mat4(1.0),glm::vec3(-tDelta,0.0,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_FORWARD]))
        view = glm::translate(glm::mat4(1.0),glm::vec3(0.0,0.0,+tDelta))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_BACKWARD]))
        view = glm::translate(glm::mat4(1.0),glm::vec3(0.0,0.0,-tDelta))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_UP]))
        view = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-tDelta,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_DOWN]))
        view = glm::translate(glm::mat4(1.0),glm::vec3(0.0,+tDelta,0.0))*view;

    // currentRotation
    if (Input::isKeyHold(keys[KEY_CAMERA_TURN_XN]))
        view = glm::rotate(glm::mat4(1.0),rDelta,glm::vec3(-1.0,0.0,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_TURN_XP]))
        view = glm::rotate(glm::mat4(1.0),rDelta,glm::vec3(+1.0,0.0,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_TURN_YN]))
        view = glm::rotate(glm::mat4(1.0),rDelta,glm::vec3(0.0,-1.0,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_TURN_YP]))
        view = glm::rotate(glm::mat4(1.0),rDelta,glm::vec3(0.0,+1.0,0.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_TURN_ZN]))
        view = glm::rotate(glm::mat4(1.0),rDelta,glm::vec3(0.0,0.0,-1.0))*view;
    if (Input::isKeyHold(keys[KEY_CAMERA_TURN_ZP]))
        view = glm::rotate(glm::mat4(1.0),rDelta,glm::vec3(0.0,0.0,+1.0))*view;


}


void LevelEditor::assignKeysMapping()
{
    keys[KEY_ESCAPE] = GLFW_KEY_ESCAPE;
    keys[KEY_CAMERA_LEFT] = GLFW_KEY_A;
    keys[KEY_CAMERA_RIGHT] = GLFW_KEY_D;
    keys[KEY_CAMERA_FORWARD] = GLFW_KEY_W;
    keys[KEY_CAMERA_BACKWARD] = GLFW_KEY_S;
    keys[KEY_CAMERA_UP] = GLFW_KEY_E;
    keys[KEY_CAMERA_DOWN] = GLFW_KEY_Q;
    keys[KEY_CAMERA_TURN_LEFT] = GLFW_KEY_J;
    keys[KEY_CAMERA_TURN_RIGHT] = GLFW_KEY_L;
    keys[KEY_CAMERA_TURN_XN] = GLFW_KEY_I;
    keys[KEY_CAMERA_TURN_XP] = GLFW_KEY_K;
    keys[KEY_CAMERA_TURN_YN] = GLFW_KEY_J;
    keys[KEY_CAMERA_TURN_YP] = GLFW_KEY_L;
    keys[KEY_CAMERA_TURN_ZN] = GLFW_KEY_U;
    keys[KEY_CAMERA_TURN_ZP] = GLFW_KEY_O;
    keys[KEY_SAVE] = GLFW_KEY_Z;
    //keys[KEY_CTRL] = GLFW_KEY_LEFT_CONTROL;
}

glm::vec3 LevelEditor::mouseIntersectCurrentPlan(glm::vec2 mouse)
{
    glm::mat4 inverse = glm::inverse(projection*view);
    glm::vec4 m1 = inverse*glm::vec4(mouse.x,mouse.y,+0.0,1.0);
    glm::vec4 m2 = inverse*glm::vec4(mouse.x,mouse.y,+1.0,1.0);

    m1 = m1/m1.w;
    m2 = m2/m2.w;

    float lambda = (m1.z+0.5)/(m1.z-m2.z);
    return glm::vec3(m1 + (m2-m1) * lambda);
}
