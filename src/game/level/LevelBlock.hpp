#include "graphic/GLContainer.hpp"
#include "utils/Texture.hpp"
#include <string>
#include <vector>

class LevelBlock
{
    public:
        // constructor
        static LevelBlock& loadFromFile(std::string filename);
        static LevelBlock& loadFromName(std::string name);

        static bool namesLoaded;
        static void loadAllNames();
        
        // getter
        const std::vector<GLContainer::Vertice>& getVertices() const;
        const std::string& getName() const;
        Texture& getTexture() const;

    private:
        // constructor
        LevelBlock(std::string filename);

        // content
        std::vector<GLContainer::Vertice> vertices;
        std::string name;
        Texture* texture;
};
