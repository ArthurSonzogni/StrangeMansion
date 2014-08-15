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
    private:
        std::vector<LevelPart*> levelPart;
};

#endif /* end of include guard: LEVEL_2OHC8ATE */
