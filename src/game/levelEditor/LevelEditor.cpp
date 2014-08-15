#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LevelEditor.hpp"
#include <iostream>
#include "graphic/Shader.hpp"
#include "utils/glError.hpp"
#include "utils/Texture.hpp"
#include "system/Input.hpp"

using namespace std;



LevelEditor::LevelEditor():
    Application(),
    obj("obj/Charmander.obj"),
    screenObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/compose.vert",
        "shader/compose.frag"
    )),
    framebuffer(width,height,3),
    level("map/1")
{
    glCheckError(__FILE__,__LINE__);
    assignKeysMapping();
    projection = glm::perspective(70.0f, 640.f/460.f, 0.1f, 100.f);
    view  = glm::lookAt(
        glm::vec3(0.0,6.0,6.0),
        glm::vec3(0.0,0.0,3.0),
        glm::vec3(0.0,0.0,1.0)
    );
}

void LevelEditor::loop()
{
    Input::update(getWindow());
    cameraEvent();

    //=================================
    framebuffer.bindToWrite();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    static float t=0;
    t+=getFrameDeltaTime();
    grid.getShader().use();
    grid.getShader().setUniform("projection", projection);
    grid.getShader().setUniform("view", view);
    grid.getShader().setUniform("model", glm::mat4(1.0));
    grid.draw();
    ShaderProgram& s(ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ));
    s.use();
    s.setUniform("projection", projection);
    s.setUniform("view", view);
    s.setUniform("model", glm::mat4(1.0));
    level.draw();

    if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
        static int ii=0;
        ii++;
        grid.setSubvision(ii);
    }

    //=================================
    if (Input::isKeyHold(GLFW_KEY_SPACE)) {
        framebuffer.drawToScreen();
    } else {
        screenObj.getShader().use();
        framebuffer.bindToRead();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenObj.getShader().setUniform("positionMap",0);
        screenObj.getShader().setUniform("colorMap",1);
        screenObj.getShader().setUniform("normalMap",2);
        screenObj.getShader().setUniform("lightDir",glm::vec3(view*glm::vec4(0.0,0.0,-1.0,0.0)));
        static float blur = 1.0;
        if (Input::isKeyHold(GLFW_KEY_A)) blur *= 1.01;
        if (Input::isKeyHold(GLFW_KEY_B)) blur *= 0.99;


        //glDepthMask(GL_FALSE);
        //glDisable(GL_DEPTH_TEST);
        //glEnable(GL_BLEND);
        //glBlendEquation(GL_FUNC_ADD);
        //glBlendFunc(GL_ONE, GL_ONE);
        
        screenObj.draw();
    }
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

    // rotation
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
}
