#include "FrameBuffer.hpp"
#include "system/Application.hpp"
#include <iostream>
#include "Shader.hpp"
#include "utils/glm.hpp"
#include "utils/glError.hpp"
#include <cmath>
using namespace std;


FrameBuffer::FrameBuffer(unsigned int _width,unsigned int _height, unsigned int colorAttachment):
    width(_width),
    height(_height),
    colorTextures(colorAttachment)
{
    allocate();
}

FrameBuffer::~FrameBuffer()
{
    disallocate();
}

void FrameBuffer::allocate()
{
    // disallocate the previous OpenGL objects
    disallocate();

    // create the framebuffer object
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);
    cout<<"fbo="<<fbo<<endl;

    // create all the textures buffers
    glGenTextures(colorTextures.size(),&colorTextures[0]);
    glGenTextures(1,&depthTexture);
    cout<<depthTexture<<endl;

    // non-depth
    for (unsigned int i = 0 ; i<colorTextures.size(); i++)
    {
        cout<<colorTextures[i]<<endl;
        glBindTexture(GL_TEXTURE_2D, colorTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTextures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    GLenum DrawBuffers[colorTextures.size()];
    for(unsigned int i = 0 ; i<colorTextures.size() ; i++) 
    {
        DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i; 
    }
    glDrawBuffers(colorTextures.size(), DrawBuffers);

    // test if opengl accept the configuration
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        cout << "[Error] FrameBuffer : status: ";
        switch(status) {
            case GL_FRAMEBUFFER_UNDEFINED: cout<<"UNDEFINED"<<endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: cout<<"INCOMPLETE_ATTACHMENT"<<endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: cout<<"INCOMPLETE_MISSING_ATTACHMENT"<<endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: cout<<"INCOMPLETE_DRAW_BUFFER"<<endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: cout<<"INCOMPLETE_READ_BUFFER"<<endl; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: cout<<"UNSUPPORTED"<<endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: cout<<"INCOMPLETE_MULTISAMPLE"<<endl; break;
            default : cout<<status<<endl;
        }
        std::exit(EXIT_FAILURE);
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBuffer::recreate(unsigned int _width,unsigned int _height)
{
   width = _width;
   height = _height;
   allocate();
}

void FrameBuffer::disallocate()
{
    if (fbo)
    {
        glDeleteRenderbuffers(1,&fbo) ;
        fbo=0;
    }
    if (colorTextures.size()>0 and colorTextures[0])
    {
        glDeleteTextures(colorTextures.size(),&colorTextures[0]);
    }
    if (depthTexture)
    {
        glDeleteTextures(1,&depthTexture);
        depthTexture = 0;
    }
}

void FrameBuffer::bindToWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void FrameBuffer::bindToRead()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    for (unsigned int i = 0 ; i < colorTextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, colorTextures[i]);
    }
}

void FrameBuffer::drawToScreen()
{
    // get window dimensions
    unsigned int width = Application::getInstance().getWidth();
    unsigned int height = Application::getInstance().getHeight();

    // clear screen and initialisations
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0);

    unsigned int div = 1;
    while(div*div<colorTextures.size()) ++div;


    unsigned int x=0;
    unsigned int y=0;
    for(unsigned int i = 0; i<colorTextures.size(); ++i) {
        // draw
        glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
        glBlitFramebuffer(
            0, 0, width, height,
            x, y, x+width/div, y+height/div,
            GL_COLOR_BUFFER_BIT, GL_LINEAR);
        // displacement
        x+=width/div;
        if (x>width-width/div+3) {
            y+=height/div;
            x=0;
        }
    }
}
