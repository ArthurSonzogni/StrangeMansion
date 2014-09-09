#ifndef LEVELEDITOR_APETXIAH
#define LEVELEDITOR_APETXIAH

#include "system/Application.hpp"
#include "graphic/FrameBuffer.hpp"
#include "graphic/ModelObj.hpp"
#include "../level/Level.hpp"
#include "Grid.hpp"
#include <map>
#include "utils/glm.hpp"

class ShaderProgram;

class LevelEditor : public Application
{
    public:
        LevelEditor();

    protected:
        void loop();
        FrameBuffer framebuffer;
        ModelObj obj;
        ModelObj screenObj;
        GLuint texture;
        Level level;
        Grid grid;
        
        void cameraEvent();

        enum Keys {
            KEY_ESCAPE,
            KEY_CAMERA_LEFT,
            KEY_CAMERA_RIGHT,
            KEY_CAMERA_FORWARD,
            KEY_CAMERA_BACKWARD,
            KEY_CAMERA_UP,
            KEY_CAMERA_DOWN,
            KEY_CAMERA_TURN_LEFT,
            KEY_CAMERA_TURN_RIGHT,
            KEY_CAMERA_TURN_XN,
            KEY_CAMERA_TURN_XP,
            KEY_CAMERA_TURN_YN,
            KEY_CAMERA_TURN_YP,
            KEY_CAMERA_TURN_ZN,
            KEY_CAMERA_TURN_ZP,
            KEY_SAVE
        };
        std::map<Keys,int> keys;
        void assignKeysMapping();

        glm::mat4 projection;
        glm::mat4 view;

        glm::vec3 mouseIntersectCurrentPlan(glm::vec2 mouse);

        void step();
        void draw();
        bool drawingMode;
        
        int currentRotation;
        std::string currentBlock;

};

#endif /* end of include guard: LEVELEDITOR_APETXIAH */
