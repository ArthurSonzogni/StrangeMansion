#ifndef LEVEL_2OHC8ATE
#define LEVEL_2OHC8ATE

#include <vector>
#include <string>
#include "utils/glm.hpp"

class LevelPart;

class Level
{
    public:
        Level(const std::string& folder);
        ~Level();

        void draw();


        // levelEditor method to edit a level.
        void addLevelPart(const std::string& name);
        void removeLevelPart(const std::string& name);
        void selectCurrentLevelPart(int n);
        void addBlock(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void addBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void save();
        bool testBlock(const glm::vec3 p0,const glm::vec3 p1);

    private:
        std::vector<LevelPart*> levelPart;
        const std::string& folder;
};

#endif /* end of include guard: LEVEL_2OHC8ATE */
