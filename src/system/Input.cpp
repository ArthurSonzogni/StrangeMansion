#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "utils/glm.hpp"

#include <map>
#include <vector>
#include <cmath>

using namespace std;

map<int,pair<int,int> > keyState;
map<int,pair<int,int> > mouseState;
double m_mouseX;
double m_mouseY;

void Input::update(GLFWwindow* window)
{
    // get screen dimension
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // update key
    for(auto it = keyState.begin(); it!=keyState.end(); ++it)
    {
        it->second.second = it->second.first;
        it->second.first  = glfwGetKey(window,it->first);
    }

    // update mouse
    for(auto it = mouseState.begin(); it!=mouseState.end(); ++it)
    {
        it->second.second = it->second.first;
        it->second.first  = glfwGetMouseButton(window,it->first);
    }
    glfwGetCursorPos(window, &m_mouseX, &m_mouseY);
}


bool Input::isKeyPressed(int key)
{
    auto p = keyState[key];
    return (( p.first  == GLFW_PRESS ) and
            ( p.second == GLFW_RELEASE ));
}

bool Input::isKeyReleased(int key)
{
    auto p = keyState[key];
    return (( p.first  == GLFW_RELEASE ) and
            ( p.second == GLFW_PRESS ));
}

bool Input::isKeyHold(int key)
{
    auto p = keyState[key];
    return ( p.first  == GLFW_PRESS );
}


bool Input::isMousePressed(int key)
{
    auto p = mouseState[key];
    return (( p.first  == GLFW_PRESS ) and
            ( p.second == GLFW_RELEASE ));
}

bool Input::isMouseReleased(int key)
{
    auto p = mouseState[key];
    return (( p.first  == GLFW_RELEASE ) and
            ( p.second == GLFW_PRESS ));
}

bool Input::isMouseHold(int key)
{
    auto p = mouseState[key];
    return ( p.first  == GLFW_PRESS);
}


double Input::mouseX()
{
    return m_mouseX;
}

double Input::mouseY()
{
    return m_mouseY;
}
