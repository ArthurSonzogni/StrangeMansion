#ifndef LEVEL_2OHC8ATE
#define LEVEL_2OHC8ATE

#include <vector>
#include <string>

class LevelPart;

class Level
{
    public:
        Level(const std::string& file);
        ~Level();
        void draw();


        // levelEditor method to edit a level.
        void addLevelPart(const std::string& name);
        void removeLevelPart(const std::string& name);
        void selectCurrentLevelPart(int n);
        void addBlock(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void addBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz);

    private:
        std::vector<LevelPart*> levelPart;
};

#endif /* end of include guard: LEVEL_2OHC8ATE */
