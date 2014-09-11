#ifndef APPLICATION3D_3KYVM1W4
#define APPLICATION3D_3KYVM1W4
#include "system/Application.hpp"
#include "graphic/FrameBuffer.hpp"
#include "graphic/ModelObj.hpp"
#include "level/Level.hpp"
#include "game/ShaderLib.hpp"

class ShaderProgram;

class Application3D : public Application
{
    public:
        Application3D();

    protected:
        void loop();
        ShaderLib shaderLib;
        FrameBuffer framebuffer;
        ModelObj obj;
        ModelObj screenObj;
        GLuint texture;
        Level level;
};

#endif /* end of include guard: APPLICATION3D_3KYVM1W4 */
