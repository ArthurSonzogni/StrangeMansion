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
    glm::mat4 transformation;
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

    private:

        std::vector<LevelBlockTransformed> levelBlockTransformed;

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
};
