#ifndef DEF_FRAMEBUFFER
#define DEF_FRAMEBUFFER


#include <GL/glew.h>

#include <vector>

class FrameBuffer
{
    public:

    FrameBuffer(unsigned int width, unsigned int height, unsigned int colorAttachment);
    ~FrameBuffer();

    void bindToWrite();
    void drawToScreen();
    void bindToRead();

    private:

    GLuint fbo;
    std::vector<GLuint> colorTextures;
    GLuint depthTexture;
};

#endif
