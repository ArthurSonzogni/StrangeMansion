#ifndef LEVEL_2OHC8ATE
#define LEVEL_2OHC8ATE

#include <vector>
#include <string>
#include "utils/glm.hpp"
#include <map>
#include <tuple>

class LevelPart;
class PortalTransformed;

class Level
{
    public:
        Level(const std::string& folder);
        ~Level();

        void draw();
        void draw(const glm::mat4& view, const glm::mat4& proj);


        // levelEditor method to edit a level.
        void addLevelPart(const std::string& name);
        void removeLevelPart(const std::string& name);
        void selectCurrentLevelPart(int n);
        void addBlock(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void drawBlockGhost(const std::string& name, float x, float y, float z, float rx, float ry, float rz);
        void save();
        bool testBlock(const glm::vec3 p0,const glm::vec3 p1);

    private:
        std::vector<LevelPart*> levelPart;
        const std::string& folder;

        void drawRecursivePortals(LevelPart& levelpart, glm::mat4 const &viewMat, glm::mat4 const &projMat, size_t maxRecursionLevel, size_t recursionLevel);

        std::map<const PortalTransformed*,const PortalTransformed*> portalDestination;
};

#endif /* end of include guard: LEVEL_2OHC8ATE */
