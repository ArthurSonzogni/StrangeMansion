#include <GL/glew.h>
#include <vector>
#include <string>
#include "utils/glm.hpp"
#include <vector>
#include "utils/Texture.hpp"
#include "graphic/GLContainer.hpp"
#include "LevelBlock.hpp"
#include "Portal.hpp"

class Portal;
class ShaderProgram;


//  _________________________
// |                         |
// |   LevelBlockTransformed |
// |_________________________|
//
struct LevelBlockTransformed
{
    LevelBlock& block;
    glm::vec3 translation;
    glm::vec3 rotation;
    static bool sortByTexture(const LevelBlockTransformed* a, const LevelBlockTransformed* b);
};

//  ______________________
// |                      |
// |   PortalTransformed  |
// |______________________|
//
struct PortalTransformed
{
    Portal& portal;
    std::string from;
    std::string to;
    glm::vec3 translation;
    glm::vec3 rotation;
};

//  ______________________
// |                      |
// |   TextureContainer   |
// |______________________|
//
struct TextureContainer
{
    Texture& texture;
    int index;
    int howMany;

    TextureContainer(const std::string& f, int i, int n):
        texture(Texture::loadFromFile(f)),
        index(i),
        howMany(n)
    {}
    TextureContainer(Texture& t, int i, int n):
        texture(t),
        index(i),
        howMany(n)
    {}

};

//  ______________
// |              |
// |   LevelPart  |
// |______________|
//
class LevelPart
{
    public:
        LevelPart(const std::string& filename);
        ~LevelPart();
        void draw();
        void addBlock(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void drawBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void save();

        bool testBlock(const glm::vec3& p0,const glm::vec3& p1);

    private:

        void build();

        int verticeNb;

        std::vector<Portal*> portal;
        GLuint vao;
        GLuint vbo;
        GLuint vaoGhost;
        GLuint vboGhost;

        ShaderProgram& shader;

        std::vector<TextureContainer> texture;
        std::vector<GLContainer::Vertice> vertice;
        std::vector<LevelBlockTransformed> levelBlockTransformed;
        std::vector<PortalTransformed> portalTransformed;

        void deleteBuffer();
        void addBlockInternal(const std::string& blockName, float x, float y, float z, float rx, float ry, float rz);
        void addPortal(std::string portalName,std::string from, std::string to,float x,float y,float z,float rx,float ry,float rz);

        const std::string filename;

};
