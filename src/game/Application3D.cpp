#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application3D.hpp"
#include <iostream>
#include "graphic/Shader.hpp"
#include "utils/glError.hpp"
#include "utils/Texture.hpp"
#include "system/Input.hpp"

using namespace std;



Application3D::Application3D():
    Application(),
    obj("obj/Charmander.obj"),
    screenObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/compose.vert",
        "shader/compose.frag"
    )),
    framebuffer(width,height,3)
{
    glCheckError(__FILE__,__LINE__);
}

void Application3D::loop()
{
    Input::update(getWindow());

    //=================================
    framebuffer.bindToWrite();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    static float t=0;
    t+=0.02;
    obj.getShader().use();
    obj.getShader().setUniform("projection", glm::perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f));
    obj.getShader().setUniform("view", glm::lookAt(
        glm::vec3(10.0*sin(t),5.0,10.0*cos(t)),
        glm::vec3(0.0,3,0.0),
        glm::vec3(0.0,1.0,0.0)
    ));

    obj.draw();

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
        static float blur = 1.0;
        if (Input::isKeyHold(GLFW_KEY_A)) blur *= 1.01;
        if (Input::isKeyHold(GLFW_KEY_B)) blur *= 0.99;

        screenObj.getShader().setUniform("blur",blur);

        //glDepthMask(GL_FALSE);
        //glDisable(GL_DEPTH_TEST);
        //glEnable(GL_BLEND);
        //glBlendEquation(GL_FUNC_ADD);
        //glBlendFunc(GL_ONE, GL_ONE);
        
        screenObj.draw();
    }
}
