#include "system/Application.hpp"
#include "graphic/FrameBuffer.hpp"
#include "graphic/ModelObj.hpp"

class ShaderProgram;

class Application3D : public Application
{
    public:
        Application3D();

    protected:
        void loop();
        FrameBuffer framebuffer;
        ModelObj obj;
        ModelObj screenObj;
        GLuint texture;
};
