#include <GL/glew.h>
#include <vector>
#include <string>
#include "utils/glm.hpp"
#include <vector>
#include "utils/Texture.hpp"
#include "graphic/GLContainer.hpp"
#include "LevelBlock.hpp"

class Portal;
class ShaderProgram;


//  _________________________
// |                         |
// |   LevelBlockTransformed |
// |_________________________|
//
struct LevelBlockTransformed
{
    LevelBlock block;
    glm::vec3 translation;
    glm::vec3 rotation;
    static bool sortByTexture(const LevelBlockTransformed* a, const LevelBlockTransformed* b);
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
        void addBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void save();

    private:


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

        void build();

        int verticeNb;

        std::vector<Portal*> portal;
        GLuint vao;
        GLuint vbo;

        ShaderProgram& shader;

        std::vector<TextureContainer> texture;
        std::vector<GLContainer::Vertice> vertice;
        std::vector<LevelBlockTransformed> levelBlockTransformed;

        void deleteBuffer();
        void addBlockInternal(const std::string& blockName, float x, float y, float z, float rx, float ry, float rz);

        const std::string filename;

};
